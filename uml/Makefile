GRAPHICS=odometer_classes.pdf		\
         odometer_encoder_usecase.pdf	\
	 odometer_rs232_usecase.pdf	\
	 odometer_serial_robustness.pdf	\
	 odometer_encoder_robustness.pdf

all: ${GRAPHICS}

doc: ${GRAPHICS}

# Targets to analyze files
%.svg: %.ums
	umlspeed -s $<

%.eps: %.dia
	dia -t eps $<

%.pdf: %.eps
	ps2pdf -dEPSFitPage -dEPSCrop $<

%.pdf: %.svg
	inkscape -z $< -A=$@

.SECONDARY: $(addsuffix .svg, $(basename ${GRAPHICS})) $(addsuffix .eps, $(basename ${GRAPHICS}))

# Clean target
clean:
	-rm -f *.svg *.eps *.pdf *.html
