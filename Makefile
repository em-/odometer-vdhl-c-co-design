DOC_SOURCES=odometro.tex
DOC_FIGURES=uml/odometer_rs232_usecase.pdf	\
            uml/odometer_encoder_usecase.pdf	\
            uml/odometer_classes.pdf		\
            uml/odometer_encoder_robustness.pdf	\
            uml/odometer_serial_getter_robustness.pdf \
            uml/odometer_serial_setter_robustness.pdf
DOC=odometro.pdf

REFERENCE=reference

SUBDIRS=hardware software uml

# Default target
all: hardware/hardware software/software
	./launch.py 2>&1 >/dev/null | diff -u $(REFERENCE) -

# Run target
run: hardware/hardware software/software
	./launch.py

reference: hardware/hardware software/software
	./launch.py 2>$(REFERENCE)

hardware/hardware:
	$(MAKE) -C hardware hardware

software/software:
	$(MAKE) -C software software

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
