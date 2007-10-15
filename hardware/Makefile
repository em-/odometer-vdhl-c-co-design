GHDL=ghdl
GHDLFLAGS= --ieee=synopsys
GHDLRUNFLAGS=

# Elaboration target
COMPONENTS=simulator_hwsw_interface.o	\
	   simulator_interrupt_controller.o \
           hardware.o		\
	   encoder.o		\
	   encoder_interface.o	\
	   output_interface.o   \
	   simulator_serial_controller.o	\
	   simulator_uart.o

hardware: $(COMPONENTS)
	$(GHDL) -e $(GHDLFLAGS) $@

# Targets to analyze files
%.o: %.vhdl
	$(GHDL) -a $(GHDLFLAGS) $<

# Syntax check target
check:
	$(GHDL) -s $(GHDLFLAGS) *.vhdl

# Clean target
clean:
	-ghdl --remove
