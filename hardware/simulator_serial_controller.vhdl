-- File: simulator_serial_controller.vhdl
-- Classe: SerialController
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL
-- Note: interfaces the system bus to the serial uart

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity simulator_serial_controller is
    generic (BASE_ADDR: std_logic_vector(15 downto 0));
    port (CLK, RST:     in  std_logic;
          RxData:       in std_logic_vector(7 downto 0);
          TxData:       out  std_logic_vector(7 downto 0);
          RxAv:         in std_logic;
          TxBusy:       in std_logic;
          ReadA:        out  std_logic;
          LoadA:        out  std_logic;
          BUS_STROBE:   in  std_logic;
          BUS_RnW:      in  std_logic;
          BUS_ADDR:     in  std_logic_vector(15 downto 0);
          BUS_DATA_IN:  in  std_logic_vector(15 downto 0);
          BUS_DATA_OUT: out std_logic_vector(15 downto 0);
          IRQ_OUT:      out std_logic);
end simulator_serial_controller;

architecture behavioral of simulator_serial_controller is
    constant DATA_ADDR:   std_logic_vector(15 downto 0) := BASE_ADDR;
    constant STATUS_ADDR: std_logic_vector(15 downto 0) := 
                                    std_logic_vector(unsigned(BASE_ADDR)+1);
  begin

  process(CLK, RST)
     variable old_rxav, old_txbusy: std_logic;
  begin
     if RST = '1' then
        IRQ_OUT <= '0';
        old_rxav   := '0';
        old_txbusy := '0';
     elsif rising_edge(CLK) then
        IRQ_OUT <= '0';
        if RxAv /= old_rxav or TxBusy /= old_txbusy then
           if RxAv = '1' or TxBusy = '0' then
              IRQ_OUT <= '1';
           end if; 
        end if;
        old_rxav   := RxAv;
        old_txbusy := TxBusy;
     end if;
  end process;

  process(CLK, RST)
  begin
     if RST = '1' then
        ReadA <= '0';
        LoadA <= '0';
     elsif rising_edge(CLK) then
        BUS_DATA_OUT <= (others => 'Z');

        -- write byte to Tx
        if (BUS_STROBE = '1' and BUS_RnW = '0' and BUS_ADDR = DATA_ADDR) then
           TxData <= BUS_DATA_IN(7 downto 0);
           LoadA <= '1';   -- Load signal
        else
           LoadA <= '0';
        end if;

        -- Read byte from Rx
        if (BUS_STROBE = '1' and BUS_RnW = '1' and BUS_ADDR = DATA_ADDR) then
            ReadA <= '1';   -- Read signal
            BUS_DATA_OUT <= (others => '0');
            BUS_DATA_OUT(7 downto 0) <= RxData;
        else
            ReadA <= '0';
        end if;

        -- Read status
        if (BUS_STROBE = '1' and BUS_RnW = '1' and BUS_ADDR = STATUS_ADDR) then
            BUS_DATA_OUT <= (others => '0');
            BUS_DATA_OUT(1 downto 0) <= TxBusy & RxAv;
        end if;
     end if;
  end process;
end behavioral;
