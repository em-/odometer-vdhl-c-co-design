DOC_SOURCES=odometro.tex
DOC_FIGURES=uml/odometer_rs232_usecase.pdf	\
            uml/odometer_encoder_usecase.pdf	\
            uml/odometer_classes.pdf		\
            uml/odometer_encoder_robustness.pdf	\
            uml/odometer_serial_getter_robustness.pdf \
            uml/odometer_serial_setter_robustness.pdf
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

%.pdf: %.svg
	inkscape -z $< -A=$@

# Generate PDF from LaTeX files
${DOC}: ${DOC_SOURCES} ${DOC_FIGURES} */*.vhdl */*.c
	rubber -f -d $<

# Build the documentation
doc: uml ${DOC}

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: subdirs $(SUBDIRS)
