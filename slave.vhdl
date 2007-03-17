library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all; -- synopsys only

entity slave is
end slave;

architecture behavioral of slave is
    signal CLK: std_logic := '0';
begin 

clock: process
begin
    CLK <= not CLK;
    wait for 0.5 ns;
end process;

pipe_interface: process
    variable RnW: std_logic;
    variable ADDR, DATA_IN, DATA_OUT: std_logic_vector(15 downto 0);

    variable l_in, l_out: line;
    variable space: character;
begin
    while true loop
        readline(input, l_in);
        report "slave read";

        read(l_in, RnW);
        read(l_in, space);

        read(l_in, ADDR);
        read(l_in, DATA_IN);

        wait for 10 ns;

        write(l_out, DATA_OUT);
        writeline(output, l_out);
    end loop;
    wait;
end process;

end behavioral;
