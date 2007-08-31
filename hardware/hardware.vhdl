-- File: hardware.vhdl
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL
-- Note: top-level hardware component

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
            IRQ:      in  std_logic;
            STROBE:   out std_logic;
            RnW:      out std_logic;
            ADDR:     out std_logic_vector(15 downto 0);
            DATA_IN:  out std_logic_vector(15 downto 0);
            FINISH:   out boolean
            );
    end component;

    component interrupt_controller
        generic (BASE_ADDR: std_logic_vector(15 downto 0));
        port(
            CLK, RST:     in  std_logic;
            REQUESTS:     in  std_logic_vector(15 downto 0);
            IRQ:          out std_logic;
            BUS_STROBE:   in  std_logic;
            BUS_RnW:      in  std_logic;
            BUS_ADDR:     in  std_logic_vector(15 downto 0);
            BUS_DATA_IN:  in  std_logic_vector(15 downto 0);
            BUS_DATA_OUT: out std_logic_vector(15 downto 0));
    end component;

    component encoder
        port (A, B, Z: out std_logic;
              FINISH: in boolean);
    end component;

    component encoder_interface
        port (CLK, RST: in std_logic;
              A, B, Z:  in std_logic;
              COUNTERCLOCKWISE, CLOCKWISE, REVOLUTION: out std_logic);
    end component;

    component output_interface
        generic (BASE_ADDR: std_logic_vector(15 downto 0));
        port (CLK, RST:     in  std_logic;
              TICK:         out std_logic;
              BUS_STROBE:   in  std_logic;
              BUS_RnW:      in  std_logic;
              BUS_ADDR:     in  std_logic_vector(15 downto 0);
              BUS_DATA_IN:  in  std_logic_vector(15 downto 0));
    end component;

    component uart
        port (CLK, RST:     in  std_logic;
              RxData:       out std_logic_vector(7 downto 0);
              TxData:       in  std_logic_vector(7 downto 0);
              RxAv:         out std_logic;
              TxBusy:       out std_logic;
              ReadA:        in  std_logic;
              LoadA:        in  std_logic);
    end component;

    component serial_controller
        generic (BASE_ADDR: std_logic_vector(15 downto 0));
        port (CLK, RST:     in  std_logic;
              RxData:       in std_logic_vector(7 downto 0);
              TxData:       out  std_logic_vector(7 downto 0);
              RxAv:         in std_logic;
              TxBusy:       in std_logic;
              ReadA:        out  std_logic;
              LoadA:        out  std_logic;
              BUS_STROBE:   in  std_logic;
              BUS_RnW:      in  std_logic;
              BUS_ADDR:     in  std_logic_vector(15 downto 0);
              BUS_DATA_IN:  in  std_logic_vector(15 downto 0);
              BUS_DATA_OUT: out std_logic_vector(15 downto 0);
              IRQ_OUT:      out std_logic);
    end component;

    signal FINISH: boolean := false;
    signal CLK: std_logic := '0';
    signal RST: std_logic;
    signal BUS_STROBE:   std_logic;
    signal BUS_RnW:      std_logic;
    signal BUS_ADDR:     std_logic_vector(15 downto 0);
    signal BUS_DATA_IN:  std_logic_vector(15 downto 0);
    signal BUS_DATA_OUT: std_logic_vector(15 downto 0);
    signal REQUESTS:     std_logic_vector(15 downto 0) := (others => '0');
    signal IRQ:          std_logic;
    signal A, B, Z:      std_logic;
    signal RxData, TxData: std_logic_vector(7 downto 0);
    signal RxAv, TxBusy: std_logic;
    signal ReadA, LoadA: std_logic;
    signal TICK:      std_logic;
begin 

clock: process
begin
    CLK <= not CLK;
    if FINISH then
        wait;
    end if;
    wait for 0.5 ns;
end process;

reset: process(CLK)
    variable resetted: boolean := false;
begin
    if rising_edge(CLK) then
        if resetted = false then
            RST <= '1';
            resetted := true;
        else
            RST <='0';
        end if;
    end if;
end process;

hwsw: hwsw_interface
    port map(CLK, RST, BUS_DATA_OUT, IRQ, BUS_STROBE, BUS_RnW, BUS_ADDR, BUS_DATA_IN, FINISH);

irq_ctrl: interrupt_controller
    generic map (BASE_ADDR => X"1000")
    port map(CLK, RST, REQUESTS, IRQ, BUS_STROBE, BUS_RnW, BUS_ADDR, BUS_DATA_IN, BUS_DATA_OUT);

enc: encoder
    port map(A, B, Z, FINISH);

enc_iface: encoder_interface
    port map(CLK, RST, A, B, Z, REQUESTS(0), REQUESTS(1), REQUESTS(2));

u: uart
    port map(CLK, RST, RxData, TxData, RxAv, TxBusy, ReadA, LoadA);

serial_iface: serial_controller
    generic map (BASE_ADDR => X"1001")
    port map(CLK, RST, RxData, TxData, RxAv, TxBusy, ReadA, LoadA,
             BUS_STROBE, BUS_RnW, BUS_ADDR, BUS_DATA_IN, BUS_DATA_OUT,
             REQUESTS(3));

output_iface: output_interface
    generic map (BASE_ADDR => X"1003")
    port map(CLK, RST, TICK, BUS_STROBE, BUS_RnW, BUS_ADDR, BUS_DATA_IN);

end behavioral;
