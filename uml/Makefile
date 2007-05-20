GRAPHICS: odometer_classes.pdf odometer_usecases.pdf

all: ${GRAPHICS}

# Targets to analyze files
%.svg: %.ums
	umlspeed -s $<

%.pdf: %.svg
	inkscape -z $< -A=$@

.SECONDARY: $(addsuffix .svg, $(basename ${GRAPHICS}))

# Clean target
clean:
	-rm -f *.svg *.pdf
