GHDL=ghdl
GHDLFLAGS= --ieee=synopsys
GHDLRUNFLAGS=

# Elaboration target
COMPONENTS=hwsw_interface.o	\
           hardware.o		\
	   encoder.o		\
	   encoder_interface.o	\
	   serial_interface.o	\
	   uart.o

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
