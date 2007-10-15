-- File: encoder.vhdl
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL
-- Note: simulates a relative encoder wit A, B, and Z signals

library ieee;
use ieee.std_logic_1164.all;

entity simulator_encoder is
    generic (tick_angle: natural := 900);
    port (A, B, Z: out std_logic;
          FINISH: in boolean);
end simulator_encoder;

architecture behavioral of simulator_encoder is
   type STATE is (S_00, S_10, S_11, S_01);
   type MOVEMENT is (NONE, COUNTERCLOCKWISE, CLOCKWISE);
   signal current_state: STATE;
   signal direction: MOVEMENT;
   signal tick: boolean := true; -- used only to generate events
   signal delay: time:= 1 ns;
begin

process
begin
    direction <= COUNTERCLOCKWISE;
    delay <= 100 ns;
    wait for 69120 ns;
    direction <= CLOCKWISE;
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
        if direction = COUNTERCLOCKWISE then
            position := (position - tick_angle) mod 3600;
        elsif direction = CLOCKWISE then
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
      if    direction = COUNTERCLOCKWISE then current_state <= S_10;
      elsif direction = CLOCKWISE        then current_state <= S_01;
      end if;

   when S_10 =>
      if    direction = COUNTERCLOCKWISE then current_state <= S_11;
      elsif direction = CLOCKWISE        then current_state <= S_00;
      end if;

   when S_11 =>
      if    direction = COUNTERCLOCKWISE then current_state <= S_01;
      elsif direction = CLOCKWISE        then current_state <= S_10;
      end if;

   when S_01 =>
      if    direction = COUNTERCLOCKWISE then current_state <= S_00;
      elsif direction = CLOCKWISE        then current_state <= S_11;
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
