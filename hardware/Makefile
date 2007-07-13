GHDL=ghdl
GHDLFLAGS= --ieee=synopsys
GHDLRUNFLAGS=

# Elaboration target
hardware: hwsw_interface.o hardware.o bus_interface.o encoder.o encoder_interface.o
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
