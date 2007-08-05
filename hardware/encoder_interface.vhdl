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
    generic (BASE_ADDR: std_logic_vector(15 downto 0));
    port (CLK, RST: in std_logic;
          A, B, Z:  in std_logic;
          STROBE:   in std_logic;
          RnW:      in std_logic;
          ADDR:     in std_logic_vector(15 downto 0);
          BUS_DATA_OUT: out std_logic_vector(15 downto 0));
end encoder_interface;

architecture behavioral of encoder_interface is
    signal STATUS: std_logic_vector (15 downto 0);
begin
process (CLK, RST)
begin
    if RST = '0' then
        STATUS <= (others => '0');
    elsif rising_edge(CLK) then
        STATUS(0) <= A;
        STATUS(1) <= B;
        STATUS(2) <= Z;
    end if;
end process;

process (CLK, RST)
begin
    if rising_edge(CLK) then
        BUS_DATA_OUT <= (others => 'Z');
    end if;
end process;
end behavioral;
