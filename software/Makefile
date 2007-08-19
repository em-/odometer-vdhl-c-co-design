SOFTWARE_SOURCES=swhw_interface.h swhw_interface.c serial_interface.h serial_interface.c

# Default target
all: software

software: software.c ${SOFTWARE_SOURCES}
	gcc -Wall -pedantic -o software $^

# Clean target
clean:
	-rm -Rf software

