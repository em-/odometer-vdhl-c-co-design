-- File: output_interface.vhdl
-- Classe:
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL

library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;

entity output_interface is
    generic (BASE_ADDR: std_logic_vector(15 downto 0));
    port (CLK, RST:     in  std_logic;
          TICK:         out std_logic;
          BUS_STROBE:   in  std_logic;
          BUS_RnW:      in  std_logic;
          BUS_ADDR:     in  std_logic_vector(15 downto 0);
          BUS_DATA_IN:  in  std_logic_vector(15 downto 0));
end output_interface;

architecture behavioral of output_interface is
begin
  process(CLK, RST)
  begin
     if RST = '1' then
        TICK <= '0';
     elsif rising_edge(CLK) then
        TICK <= '0';
	if (BUS_STROBE = '1' and BUS_RnW = '0' and BUS_ADDR = BASE_ADDR) then
            TICK <= '1';
        end if;
     end if;
  end process;
end behavioral;
