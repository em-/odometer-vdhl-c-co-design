SOFTWARE_SOURCES=swhw_interface.c	\
		 serial_interface.c	\
		 odometer.c

# Default target
all: software

software: software.c ${SOFTWARE_SOURCES}
	gcc -Wall -pedantic -o software $^

# Clean target
clean:
	-rm -Rf software

