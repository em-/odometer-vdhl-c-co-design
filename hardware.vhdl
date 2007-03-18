library std;
library ieee;

use std.textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all; -- synopsys only

entity hardware is
end hardware;

architecture behavioral of hardware is
    component hwsw_interface
        port(
            CLK, RST: in  std_logic;
            DATA_OUT: in  std_logic_vector(15 downto 0);
            STROBE:   out std_logic;
            RnW:      out std_logic;
            ADDR:     out std_logic_vector(15 downto 0);
            DATA_IN:  out std_logic_vector(15 downto 0)
            );
    end component;

    signal CLK: std_logic := '0';
    signal RST: std_logic;
    signal BUS_STROBE:   std_logic;
    signal BUS_RnW:      std_logic;
    signal BUS_ADDR:     std_logic_vector(15 downto 0);
    signal BUS_DATA_IN:  std_logic_vector(15 downto 0);
    signal BUS_DATA_OUT: std_logic_vector(15 downto 0);
begin 

clock: process
begin
    CLK <= not CLK;
    wait for 0.5 ns;
end process;

hwsw: hwsw_interface
    port map(CLK, RST, BUS_DATA_OUT, BUS_STROBE, BUS_RnW, BUS_ADDR, BUS_DATA_IN);

end behavioral;
