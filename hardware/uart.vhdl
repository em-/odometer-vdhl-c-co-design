-- File: uart.vhdl
-- Classe:
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL

library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all; -- synopsys only

entity uart is
    port (CLK, RST:     in  std_logic;
          Rx:           in  std_logic;
          Tx:           out std_logic;
          BUS_STROBE:   in  std_logic;
          BUS_RnW:      in  std_logic;
          BUS_ADDR:     in  std_logic_vector(15 downto 0);
          BUS_DATA_IN:  in  std_logic_vector(15 downto 0);
          BUS_DATA_OUT: out std_logic_vector(15 downto 0));
end uart;

architecture behavioral of uart is
  signal RxData : std_logic_vector(7 downto 0); -- Last Byte received
  signal TxData : std_logic_vector(7 downto 0); -- Last bytes transmitted
  signal RxAv   : std_logic;  -- Data Received
  signal TxBusy : std_logic;  -- Transmiter Busy
  signal ReadA  : std_logic;  -- Async Read receive buffer
  signal LoadA  : std_logic;  -- Async Load transmit buffer

  begin

  process
  begin
    RxData <= (others => '0');
    wait for 10 ns;
    RxData <= "00000001";
    wait;
  end process;

  process
  begin
    wait on LoadA;
    if rising_edge(LoadA) then
        TxBusy <= '1';
        wait for 125 us;
        TxBusy <= '0';
    end if;
  end process;

  process(RxData, ReadA)
  begin
    if RxData'event then
        RxAv <= '1';
    elsif rising_edge(ReadA) then
        RxAv <= '0';
    end if;
  end process;


  process(CLK)
  begin
     if rising_edge(CLK) then
        if (RST = '1') then
  	   ReadA <= '0';
  	   LoadA <= '0';
  	else
           BUS_DATA_OUT <= (others => 'Z');

	   if (BUS_STROBE = '1' and BUS_RnW = '0' and BUS_ADDR = "00") then -- write byte to Tx
              TxData <= BUS_DATA_IN(7 downto 0);
	      LoadA <= '1';   -- Load signal
           else
              LoadA <= '0';
           end if;

	   if (BUS_STROBE = '1' and BUS_RnW = '1' and BUS_ADDR = "00") then -- Read byte from Rx
	       ReadA <= '1';   -- Read signal
               BUS_DATA_OUT <= (others => '0');
               BUS_DATA_OUT(7 downto 0) <= RxData;
           else
               ReadA <= '0';
           end if;

	   if (BUS_STROBE = '1' and BUS_RnW = '1' and BUS_ADDR = "01") then -- Read status
               BUS_DATA_OUT <= (others => '0');
               BUS_DATA_OUT(1 downto 0) <= TxBusy & RxAv;
           end if;
        end if;
     end if;
  end process;
end behavioral;
