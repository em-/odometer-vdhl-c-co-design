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
	type STATE is (START_00,
                       LR_10, LR_11, LR_01,
                       RL_01, RL_11, RL_10);
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
        current_state <= START_00;
        LEFT  <= '0';
        RIGHT <= '0';
    elsif rising_edge(CLK) then
        Quad <= A & B;

        LEFT  <= '0';
        RIGHT <= '0';

        case current_state is
        when START_00 =>
            case Quad is
                when "01" => current_state <= LR_01;
                when "10" => current_state <= LR_10;
                when others => current_state <= START_00;
            end case;

        when LR_10 =>
            case Quad is
                when "00" => current_state <= START_00;
                when "10" => current_state <= LR_10;
                when "11" => current_state <= LR_11;
                when others => current_state <= START_00;
            end case;

        when LR_11 =>
            case Quad is
                when "00" => current_state <= LR_11; --?
                when "01" => current_state <= LR_01;
                when "10" => current_state <= LR_10;
                when "11" => current_state <= LR_11; --?
                when others => current_state <= START_00;
            end case;

        when LR_01 =>
            case Quad is
                when "00" => 
                    current_state <= START_00;
                    LEFT <= '1';
                when "01" => current_state <= LR_01;
                when "11" => current_state <= LR_11;
                when others => current_state <= START_00;
            end case;

        when RL_01 =>
            case Quad is
                when "00" => current_state <= START_00;
                when "01" => current_state <= RL_01;
                when "11" => current_state <= RL_11;
                when others => current_state <= START_00;
            end case;

        when RL_11 =>
            case Quad is
                when "01" => current_state <= RL_01;
                when "10" => current_state <= RL_10;
                when "11" => current_state <= RL_11;
                when others => current_state <= START_00;
            end case;

        when RL_10 =>
            case Quad is
                when "00" =>
                    current_state <= START_00;
                    RIGHT <= '1';
                when "10" => current_state <= RL_10;
                when "11" => current_state <= RL_11;
                when others => current_state <= START_00;
            end case;

        when others =>
            current_state <= START_00;

        end case;
    end if;
end process;
end behavioral;
