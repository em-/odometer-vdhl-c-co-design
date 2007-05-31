-- File: bus_interface.vhdl
-- Classe:
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL

library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all; -- synopsys only

entity bus_interface is
    port (
        CLK, RST:     in  std_logic;
        BUS_STROBE:   in  std_logic;
        BUS_RnW:      in  std_logic;
        BUS_ADDR:     in  std_logic_vector(15 downto 0);
        BUS_DATA_IN:  in  std_logic_vector(15 downto 0);
        BUS_DATA_OUT: out std_logic_vector(15 downto 0)
        );
end bus_interface;

architecture behavioral of bus_interface is
begin
process(CLK)
begin
    if rising_edge(CLK) and BUS_STROBE = '1' then
        case BUS_ADDR is
            when "0000000000000000" =>
                if BUS_RnW = '1' then
                    BUS_DATA_OUT <= "0100001100100001";
                else
                    -- read BUS_DATA_IN and do something
                end if;
        end case;
    end if;
end process;
end behavioral;
