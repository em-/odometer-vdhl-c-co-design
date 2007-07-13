-- File: encoder_interface.vhdl
-- Classe:
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL

library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all; -- synopsys only

entity encoder_interface is
    port (CLK, RST: in std_logic;
          A, B, Z:  in std_logic;
          STROBE:   in std_logic;
          RnW:      in std_logic;
          ADDR:     in std_logic_vector(15 downto 0);
          BUS_DATA_OUT: out std_logic_vector(15 downto 0));
end encoder_interface;

architecture behavioral of encoder_interface is
begin
process (CLK, RST)
begin
    if RST = '0' then
        BUS_DATA_OUT <= (others => 'Z');
    elsif rising_edge(CLK) then
        BUS_DATA_OUT <= (others => 'Z');
    end if;
end process;
end behavioral;
