SOFTWARE_SOURCES=software.c		\
 		 simulator_swhw_interface.c	\
		 serial_interface.c	\
		 odometer.c

SOFTWARE_HEADERS=memory_map.h		\
		 util.h			\
		 simulator_swhw_interface.h	\
		 serial_interface.h	\
		 odometer.c		\

# Default target
all: software

software: ${SOFTWARE_SOURCES} ${SOFTWARE_HEADERS}
	gcc -std=gnu99 -Wall -pedantic -o software ${SOFTWARE_SOURCES}

# Clean target
clean:
	-rm -Rf software

