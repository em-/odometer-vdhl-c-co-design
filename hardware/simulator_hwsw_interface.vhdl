-- File: simulator_hwsw_interface.vhdl
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL
-- Note: Software <-> Hardware Interface communicating through stdin/stdout

library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all; -- synopsys only

entity simulator_hwsw_interface is
    port(
        CLK, RST: in  std_logic;
        DATA_OUT: in  std_logic_vector(15 downto 0);
        IRQ:      in  std_logic;
        STROBE:   out std_logic;
        RnW:      out std_logic;
        ADDR:     out std_logic_vector(15 downto 0);
        DATA_IN:  out std_logic_vector(15 downto 0);
        FINISH:   out boolean
        );
end simulator_hwsw_interface;

architecture behavioral of simulator_hwsw_interface is
    signal counter: integer := 0;
    constant DEBUG: boolean := false;
begin

-- For each clock tick increment the mod 10 counter
count: process(CLK)
begin
    if rising_edge(CLK) then
        counter <= (counter + 1) mod 10;
    end if;
end process;

-- Implements the stdin/stdout communication pipe with the sw/hw interface
pipe_interface: process
    variable t_STROBE:  std_logic;
    variable t_RnW:     std_logic;
    variable t_ADDR:    std_logic_vector(15 downto 0);
    variable t_DATA_IN: std_logic_vector(15 downto 0);
    variable l_in, l_out: line;
    variable space: character;
begin
    FINISH <= false;
    while true loop
        -- Set FINISH if the pipe has been closed
        if endfile(input) then
            FINISH <= true;
            wait;
        end if;

        -- Block until a line has been read from stdin
        readline(input, l_in);

        if DEBUG then
            report "slave read";
        end if;

        -- Decode the bus signals
        read(l_in, t_STROBE);
        read(l_in, t_RnW);
        read(l_in, space);

        read(l_in, t_ADDR);
        read(l_in, t_DATA_IN);

        STROBE  <= t_STROBE;
        Rnw     <= t_RnW;
        ADDR    <= t_ADDR;
        DATA_IN <= t_DATA_IN;

        -- Keep the STROBE signal high for a single clock cycle only
        wait on counter;
        STROBE <= '0';

        -- Write on stdout the hardware response
        write(l_out, DATA_OUT);
        write(l_out, space);
        write(l_out, IRQ);
        writeline(output, l_out);

        -- Sleep for the next 10 clock cycles
        wait on counter until counter = 1;
    end loop;
    wait;
end process;

end behavioral;
