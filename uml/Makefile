GRAPHICS=odometer_classes.pdf		\
         odometer_encoder_usecase.pdf	\
	 odometer_rs232_usecase.pdf

all: ${GRAPHICS}

doc: ${GRAPHICS}

# Targets to analyze files
%.svg: %.ums
	umlspeed -s $<

%.pdf: %.svg
	inkscape -z $< -A=$@

.SECONDARY: $(addsuffix .svg, $(basename ${GRAPHICS}))

# Clean target
clean:
	-rm -f *.svg *.pdf
