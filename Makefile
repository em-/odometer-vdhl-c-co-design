GHDL=ghdl
GHDLFLAGS= --ieee=synopsys
GHDLRUNFLAGS=

# Default target
all: run


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
run: slave
	./launch.py

# Clean target
clean:
	-ghdl --remove
