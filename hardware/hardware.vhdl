-- File: hardware.vhdl
-- Classe:
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL

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
            IRQ:      in  std_logic_vector(7 downto 0);
            STROBE:   out std_logic;
            RnW:      out std_logic;
            ADDR:     out std_logic_vector(15 downto 0);
            DATA_IN:  out std_logic_vector(15 downto 0);
            FINISH:   out boolean
            );
    end component;

    component encoder
        port (A, B, Z: out std_logic);
    end component;

    component encoder_interface
        generic (BASE_ADDR: std_logic_vector(15 downto 0));
        port (CLK, RST: in std_logic;
              A, B, Z:  in std_logic;
              LEFT, RIGHT, REVOLUTION: out std_logic);
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

    component serial_interface
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
              BUS_DATA_OUT: out std_logic_vector(15 downto 0));
    end component;

    signal FINISH: boolean := false;
    signal CLK: std_logic := '0';
    signal RST: std_logic;
    signal BUS_STROBE:   std_logic;
    signal BUS_RnW:      std_logic;
    signal BUS_ADDR:     std_logic_vector(15 downto 0);
    signal BUS_DATA_IN:  std_logic_vector(15 downto 0);
    signal BUS_DATA_OUT: std_logic_vector(15 downto 0);
    signal IRQ:          std_logic_vector(7 downto 0) := "00000000";
    signal A, B, Z:      std_logic;
    signal RxData, TxData: std_logic_vector(7 downto 0);
    signal RxAv, TxBusy: std_logic;
    signal ReadA, LoadA: std_logic;
begin 

clock: process
begin
    CLK <= not CLK;
    if FINISH then
        wait;
    end if;
    wait for 0.5 ns;
end process;

hwsw: hwsw_interface
    port map(CLK, RST, BUS_DATA_OUT, IRQ, BUS_STROBE, BUS_RnW, BUS_ADDR, BUS_DATA_IN, FINISH);

enc: encoder
    port map(A, B, Z);

enc_iface: encoder_interface
    generic map (BASE_ADDR => X"0000")
    port map(CLK, RST, A, B, Z, IRQ(0), IRQ(1), IRQ(2));

u: uart
    port map(CLK, RST, RxData, TxData, RxAv, TxBusy, ReadA, LoadA);

serial_iface: serial_interface
    generic map (BASE_ADDR => X"0001")
    port map(CLK, RST, RxData, TxData, RxAv, TxBusy, ReadA, LoadA,
             BUS_STROBE, BUS_RnW, BUS_ADDR, BUS_DATA_IN, BUS_DATA_OUT);

end behavioral;
