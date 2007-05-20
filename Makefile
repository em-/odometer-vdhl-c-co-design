DOC_SOURCES=odometro.tex
DOC_FIGURES=
DOC=odometro.pdf

SUBDIRS=hardware software

# Default target
all: subdirs run

# Run target
run: hardware/hardware software/software
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
${DOC}: ${DOC_SOURCES} ${DOC_FIGURES_PDF} *.vhdl
	rubber -d $<

# Build the documentation
doc: ${DOC}

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: subdirs $(SUBDIRS)
