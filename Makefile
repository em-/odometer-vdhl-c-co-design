GHDL=ghdl
GHDLFLAGS= --ieee=synopsys
GHDLRUNFLAGS=

# Default target
all: run

software: software.c
	gcc -Wall -pedantic -o software software.c

# Elaboration target
hardware: hwsw_interface.o hardware.o
	$(GHDL) -e $(GHDLFLAGS) $@

# Targets to analyze files
%.o: %.vhdl
	$(GHDL) -a $(GHDLFLAGS) $<

# Syntax check target
check:
	$(GHDL) -s $(GHDLFLAGS) */*.vhdl

# Run target
run: hardware software
	./launch.py

# Clean target
clean:
	-ghdl --remove
	-rm -Rf software
