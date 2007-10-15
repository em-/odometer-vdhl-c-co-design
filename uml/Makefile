GRAPHICS=odometer_classes.pdf		\
         odometer_encoder_usecase.pdf	\
	 odometer_rs232_usecase.pdf	\
	 odometer_serial_getter_robustness.pdf	\
	 odometer_serial_setter_robustness.pdf	\
	 odometer_encoder_robustness.pdf

all: ${GRAPHICS}

doc: ${GRAPHICS}

%.eps: %.dia
	dia -t eps $<

%.pdf: %.eps
	ps2pdf -dEPSFitPage -dEPSCrop $<

.SECONDARY: $(addsuffix .eps, $(basename ${GRAPHICS}))

# Clean target
clean:
	-rm -f *.eps *.pdf
