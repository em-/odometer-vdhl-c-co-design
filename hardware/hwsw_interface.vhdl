-- File: hwsw_interface.vhdl
-- Classe:
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL
-- Note: Software <-> Hardware Interface communicating through stdin/stdout

library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all; -- synopsys only

entity hwsw_interface is
    port(
        CLK, RST: in  std_logic;
        DATA_OUT: in  std_logic_vector(15 downto 0);
        STROBE:   out std_logic;
        RnW:      out std_logic;
        ADDR:     out std_logic_vector(15 downto 0);
        DATA_IN:  out std_logic_vector(15 downto 0);
        FINISH:   out boolean
        );
end hwsw_interface;

architecture behavioral of hwsw_interface is
    signal counter: integer := 0;
begin

count: process(CLK)
begin
    if rising_edge(CLK) then
        counter <= (counter + 1) mod 10;
    end if;
end process;

pipe_interface: process
    variable t_RnW:     std_logic;
    variable t_ADDR:    std_logic_vector(15 downto 0);
    variable t_DATA_IN: std_logic_vector(15 downto 0);
    variable l_in, l_out: line;
    variable space: character;
begin
    FINISH <= false;
    while true loop
        if endfile(input) then
            FINISH <= true;
            wait;
        end if;
        readline(input, l_in);
        report "slave read";

        read(l_in, t_RnW);
        read(l_in, space);

        read(l_in, t_ADDR);
        read(l_in, t_DATA_IN);

        Rnw     <= t_RnW;
        ADDR    <= t_ADDR;
        DATA_IN <= t_DATA_IN;

        STROBE <= '1';
        while counter /= 0 loop
            wait on counter;
            STROBE <= '0';
        end loop;

        write(l_out, DATA_OUT);
        writeline(output, l_out);

        wait on counter;
    end loop;
    wait;
end process;

end behavioral;
