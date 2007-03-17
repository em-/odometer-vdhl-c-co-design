GHDL=ghdl
GHDLFLAGS= --ieee=synopsys
GHDLRUNFLAGS=

# Default target
all: run

master: master.c
	gcc -Wall -pedantic -o master master.c

# Elaboration target
slave: slave.o
	$(GHDL) -e $(GHDLFLAGS) $@

# Targets to analyze files
%.o: %.vhdl
	$(GHDL) -a $(GHDLFLAGS) $<

# Syntax check target
check:
	$(GHDL) -s $(GHDLFLAGS) */*.vhdl

# Run target
run: slave master
	./launch.py

# Clean target
clean:
	-ghdl --remove
	-rm -Rf master
