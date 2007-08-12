-- File: encoder.vhdl
-- Classe:
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL

library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all; -- synopsys only

entity encoder is
    port (A, B, Z: out std_logic);
end encoder;

architecture behavioral of encoder is
begin
process
begin
    A <= '0';
    B <= '0';
    wait for 1 us;
    A <= '1';
    wait for 5 us;
    B <= '1';
    wait for 2 us;
    A <= '0';
    wait for 5 us;
    B <= '0';
    report "foo";
    wait for 2 us;
    A <= '1';
    wait for 5 us;
    B <= '1';
    wait for 2 us;
    A <= '0';
    wait;
end process;
end behavioral;
