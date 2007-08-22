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
          LEFT, RIGHT, REVOLUTION: out std_logic);
end encoder_interface;

architecture behavioral of encoder_interface is
	type STATE is (S_00, S_10, S_11, S_01);
        signal current_state: STATE;
        signal Quad: std_logic_vector(1 downto 0);
begin

process (CLK, RST)
    variable OLD_Z: std_logic;
begin
    if RST = '1' then
        OLD_Z := '0';
        REVOLUTION <= '0';
    elsif rising_edge(CLK) then
        if (OLD_Z /= Z) then
            REVOLUTION <= Z;
            OLD_Z := Z;
        else
            REVOLUTION <= '0';
        end if;
    end if;
end process;

process (CLK, RST)
begin
    if RST = '1' then
        current_state <= S_00;
        LEFT  <= '0';
        RIGHT <= '0';
    elsif rising_edge(CLK) then
        Quad <= A & B;

        LEFT  <= '0';
        RIGHT <= '0';

        case current_state is
        when S_00 =>
            case Quad is
                when "01" => current_state <= S_01;
                when "10" => current_state <= S_10;
                when others =>
            end case;

        when S_10 =>
            case Quad is
                when "00" => current_state <= S_00;
                             RIGHT <= '1';
                when "11" => current_state <= S_11;
                when others =>
            end case;

        when S_11 =>
            case Quad is
                when "01" => current_state <= S_01;
                when "10" => current_state <= S_10;
                when others =>
            end case;

        when S_01 =>
            case Quad is
                when "00" => current_state <= S_00;
                             LEFT <= '1';
                when "11" => current_state <= S_11;
                when others =>
            end case;

        when others =>
            current_state <= S_00;

        end case;
    end if;
end process;
end behavioral;
