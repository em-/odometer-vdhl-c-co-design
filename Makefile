DOC_SOURCES=odometro.tex
DOC_FIGURES=uml/odometer_rs232_usecase.svg	\
            uml/odometer_encoder_usecase.svg	\
	    uml/odometer_classes.svg
DOC=odometro.pdf

SUBDIRS=hardware software uml

# Default target
all: run

# Run target
run: hardware software hardware/hardware software/software
	./launch.py

# Clean target
clean: subdirs
	-rubber -d --clean ${DOC_SOURCES}

# Documentation rules
DOC_FIGURES_PDF=$(DOC_FIGURES:.svg=.pdf)
${DOC_FIGURES_PDF}: ${DOC_FIGURES}

%.pdf: %.svg
	inkscape -z $< -A=$@

# Generate PDF from LaTeX files
${DOC}: ${DOC_SOURCES} ${DOC_FIGURES_PDF} */*.vhdl */*.c
	rubber -d $<

# Build the documentation
doc: uml ${DOC}

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: subdirs $(SUBDIRS)
