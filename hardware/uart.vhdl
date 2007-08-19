-- File: uart.vhdl
-- Classe:
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL

library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all; -- synopsys only

entity uart is
    port (CLK, RST:     in  std_logic;
          RxData:       out std_logic_vector(7 downto 0);
          TxData:       in  std_logic_vector(7 downto 0);
          RxAv:         out std_logic;
          TxBusy:       out std_logic;
          ReadA:        in  std_logic;
          LoadA:        in  std_logic);
end uart;

architecture behavioral of uart is
    signal Data: std_logic_vector(7 downto 0);
    signal new_data, busy: boolean;
  begin

  load: process
    variable loadedData: std_logic_vector(7 downto 0);
    file test_file: text is in "hardware/serial.input";

    variable l: line;
    variable t: time;
    variable i: integer;
    variable good: boolean;
    variable space: character;
  begin
    while not endfile(test_file) loop
        readline(test_file, l);

        -- read the time from the beginning of the line
        -- skip the line if it doesn't start with an integer
        read(l, i, good => good);
        next when not good;

        read(l, space);
        read(l, loadedData);

        t := i * 10 ns;  -- convert an integer to time
        if (now < t) then
            wait for t - now + 1 ns; -- desynchronize a bit the transitions
        end if;

        Data <= loadedData;
    end loop;
    wait;
  end process;

  process
  begin
    new_data <= false;
    wait on Data;
    new_data <= true;
    wait on CLK;
    while not rising_edge(CLK) loop
        wait on CLK;
    end loop;
  end process;

  process
  begin
    busy <= false;
    wait on LoadA;
    if rising_edge(LoadA) then
        busy <= true;
        wait for 125 us;
        busy <= false;
    end if;
  end process;

  process(RST, CLK)
    variable unread_data: boolean;
  begin
    if RST = '1' then
        RxAv <= '0';
        RxData <= (others => '0');
        TxBusy <= '0';
        unread_data := false;
    elsif rising_edge(CLK) then
        if ReadA = '1' then
            unread_data := false;
            RxAv <= '0';
        end if;

        if not unread_data and new_data then
            RxData <= Data;
            RxAv <= '1';
            unread_data := true;
        end if;

        if busy then
            TxBusy <= '1';
        else
            TxBusy <= '0';
        end if;
    end if;
  end process;
end behavioral;
