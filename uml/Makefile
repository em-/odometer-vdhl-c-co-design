%.eps: %.dia
	dia -t eps $<

%.pdf: %.eps
	ps2pdf -dEPSFitPage -dEPSCrop $<

.SECONDARY: $(addsuffix .eps, $(basename ${GRAPHICS}))

# Clean target
clean:
	-rm -f *.eps *.pdf
