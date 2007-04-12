-- File: odometer.vhdl
-- Classe:
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL

library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all; -- synopsys only

entity odometer is
    port (
        CLK, RST:     in  std_logic;
        BUS_STROBE:   in  std_logic;
        BUS_RnW:      in  std_logic;
        BUS_ADDR:     in  std_logic_vector(15 downto 0);
        BUS_DATA_IN:  in  std_logic_vector(15 downto 0);
        BUS_DATA_OUT: out std_logic_vector(15 downto 0)
        );
end odometer;

architecture behavioral of odometer is
begin
end behavioral;
