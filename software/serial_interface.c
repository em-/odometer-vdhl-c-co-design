#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "simulator_swhw_interface.h"
#include "serial_interface.h"
#include "memory_map.h"

/*
 * File: serial_interface.c
 * Classe: SerialInterface
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 */

#define SERIAL_STATUS_RXAV   0x1
#define SERIAL_STATUS_TXBUSY 0x2

SerialInterface serial_interface;

static void serial_received_data(unsigned char serial_data)
{
    static int bytes_left = 0;
    static int command, command_data;

    if (bytes_left == 0) {
        command = serial_data;
        command_data = 0;
        if (command < serial_interface.handlers_size) {
            bytes_left = 
                serial_interface.handlers[command].serial_data_size;
        }
    } else {
        command_data = command_data << 8;
        command_data = command_data | serial_data;
        bytes_left--;
    }

    if (bytes_left == 0 && command < serial_interface.handlers_size) {
        serial_interface.handlers[command].handler(command_data);
    }
}

static void serial_line_free()
{
    if (serial_interface.outstanding_data >= 0) {
        bus_write(SERIAL_DATA_ADDR, serial_interface.outstanding_data);
        serial_interface.outstanding_data = -1;
    }
}

void serial_init()
{
    serial_interface.handlers_size = 0;
    serial_interface.handlers = NULL;
    serial_interface.outstanding_data = -1;
}

void serial_set_command_handlers(SerialHandler *array,
                                 int size)
{
    serial_interface.handlers = array;
    serial_interface.handlers_size = size;
}

void serial_send(int serial_data)
{
    /* send the high byte first and store the lower one */
    serial_interface.outstanding_data = serial_data & 0xFF;
    serial_data = (serial_data >> 1) & 0xFF;
    bus_write(SERIAL_DATA_ADDR, serial_data);
}

void serial_notify(void *data)
{
    int status, serial_data;

    status = bus_read(SERIAL_STATUS_ADDR);
    fprintf(stderr, "serial status %d\n", status);

    if (!(status & SERIAL_STATUS_TXBUSY))
    {
      serial_line_free(serial_interface);
    }

    if (status & SERIAL_STATUS_RXAV)
    {
      serial_data = bus_read(SERIAL_DATA_ADDR);
      serial_received_data(serial_data);
    }
}
