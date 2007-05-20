GHDL=ghdl
GHDLFLAGS= --ieee=synopsys
GHDLRUNFLAGS=

DOC_SOURCES=odometro.tex
DOC_FIGURES=
DOC=odometro.pdf

SOFTWARE_SOURCES=swhw_interface.h swhw_interface.c

# Default target
all: run

software: software.c ${SOFTWARE_SOURCES}
	gcc -Wall -pedantic -o software $^

# Elaboration target
hardware: hwsw_interface.o hardware.o odometer.o
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

# Documentation rules
DOC_FIGURES_PDF=$(DOC_FIGURES:.svg=.pdf)
${DOC_FIGURES_PDF}: ${DOC_FIGURES}

%.pdf: %.svg
	inkscape -z $< -A=$@

# Generate PDF from LaTeX files
${DOC}: ${DOC_SOURCES} ${DOC_FIGURES_PDF} *.vhdl
	rubber -d $<

# Build the documentation
doc: ${DOC}
