all:

# Targets to analyze files
%.svg: %.ums
	umlspeed -s $<

%.pdf: %.svg
	inkscape -z $< -A=$@

# Clean target
clean:
	-rm -f *.svg
