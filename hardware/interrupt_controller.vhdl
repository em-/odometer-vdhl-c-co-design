-- File: interrupt_controller.vhdl
-- Classe:
-- Autore: Emanuele Aina <em@nerd.ocracy.org>
-- Licenza: LGPL
-- Note: Simple interrupt controller

library ieee;
use ieee.std_logic_1164.all;

entity interrupt_controller is
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
end interrupt_controller;

architecture behavioral of interrupt_controller is
   constant REG_ADDR:  std_logic_vector(15 downto 0) := BASE_ADDR;
   signal outstanding: std_logic_vector(15 downto 0);
   signal clear:       std_logic_vector(15 downto 0);
begin
   outstanding_logic:
   for i in REQUESTS'range generate
      process (RST, REQUESTS(i), clear(i))
      begin
         if RST = '1' then
            outstanding(i) <= '0';
         elsif clear(i) = '1' then
            outstanding(i) <= '0';
         elsif rising_edge(REQUESTS(i)) then
            outstanding(i) <= '1';
         end if;
      end process;
   end generate outstanding_logic;
   
   irq_logic:
   process (outstanding)
   begin
      IRQ <= '0';
      for i in outstanding'range loop
         if outstanding(i) = '1' then
            IRQ <= '1';
         end if;
      end loop;
   end process irq_logic;
   
   bus_logic:
   process(CLK, RST)
   begin
      BUS_DATA_OUT <= (others => 'Z');
      clear <= (others => '0');
  
      if rising_edge(CLK) then
         -- Read the mask for the IRQs that have been handled
         if BUS_STROBE = '1' and BUS_RnW = '0' and BUS_ADDR = REG_ADDR then
            clear <= BUS_DATA_IN;
         end if;

         -- Return the currently ourstanding IRQs
         if BUS_STROBE = '1' and BUS_RnW = '1' and BUS_ADDR = REG_ADDR then
            BUS_DATA_OUT <= outstanding;
         end if;
      end if;
   end process;
end architecture behavioral;
