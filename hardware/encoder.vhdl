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
    generic (tick_angle: natural := 900);
    port (A, B, Z: out std_logic;
          FINISH: in boolean);
end encoder;

architecture behavioral of encoder is
   type STATE is (S_00, S_10, S_11, S_01);
   type MOVEMENT is (NONE, LEFT, RIGHT);
   signal current_state: STATE;
   signal direction: MOVEMENT;
   signal tick: boolean := true; -- used only to generate events
   signal delay: time:= 1 ns;
begin

process
begin
    direction <= LEFT;
    delay <= 100 ns;
    wait for 2410 ns;
    direction <= RIGHT;
    wait;
end process;

process
begin
   if FINISH then
      wait;
   else
      wait for delay;
      tick <= not tick;
   end if;
end process;

process(current_state)
    variable position: natural := 0;
    variable t: integer := 0;
begin
    if (current_state = S_00) then
        if direction = LEFT then
            position := (position - tick_angle) mod 3600;
        elsif direction = RIGHT then
            position := (position + tick_angle) mod 3600;
        end if;

        if position = 0 then
            Z <= '1';
        else
            Z <= '0';
        end if;
    end if;
end process;

process(tick)
begin
   case current_state is
   when S_00 =>
      if    direction = LEFT  then current_state <= S_10;
      elsif direction = RIGHT then current_state <= S_01;
      end if;

   when S_10 =>
      if    direction = LEFT  then current_state <= S_11;
      elsif direction = RIGHT then current_state <= S_00;
      end if;

   when S_11 =>
      if    direction = LEFT  then current_state <= S_01;
      elsif direction = RIGHT then current_state <= S_10;
      end if;

   when S_01 =>
      if    direction = LEFT  then current_state <= S_00;
      elsif direction = RIGHT then current_state <= S_11;
      end if;

   when others =>
       current_state <= S_00;
   end case;
end process;

process(current_state)
begin
   case current_state is
   when S_00 => A<='0'; B<='0';
   when S_01 => A<='0'; B<='1';
   when S_11 => A<='1'; B<='1';
   when S_10 => A<='1'; B<='0';
   end case;
end process;

end behavioral;
