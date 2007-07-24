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
          RxData:       out std_logic_vector(7 downto 0);
          TxData:       in  std_logic_vector(7 downto 0);
          RxAv:         out std_logic;
          TxBusy:       out std_logic;
          ReadA:        in  std_logic;
          LoadA:        in  std_logic;
          Rx:           in  std_logic;
          Tx:           out std_logic);
end uart;

architecture behavioral of uart is
    signal Data: std_logic_vector(7 downto 0);
  begin

  process
  begin
    Data <= (others => '0');
    wait for 10 ns;
    Data <= "00000001";
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

  process(Data, ReadA)
  begin
    if Data'event then
        RxAv <= '1';
        RxData <= Data;
    elsif rising_edge(ReadA) then
        RxAv <= '0';
    end if;
  end process;
end behavioral;
