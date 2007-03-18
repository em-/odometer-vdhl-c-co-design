GHDL=ghdl
GHDLFLAGS= --ieee=synopsys
GHDLRUNFLAGS=

SOFTWARE_SOURCES=swhw_interface.h swhw_interface.c

# Default target
all: run

software: software.c ${SOFTWARE_SOURCES}
	gcc -Wall -pedantic -o software $^

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
