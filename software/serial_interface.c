#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "swhw_interface.h"
#include "serial_interface.h"

/*
 * File: serial_interface.c
 * Classe:
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 */

#define SERIAL_DATA_ADDR   (void*)0x0002
#define SERIAL_STATUS_ADDR (void*)0x0003
#define SERIAL_STATUS_RXAV   0x1
#define SERIAL_STATUS_TXBUSY 0x2

SerialInterface serial_interface;

static void serial_received_data(unsigned char data)
{
    static int bytes_left = 0;
    static int command, command_data;

    if (bytes_left == 0) {
        command = data;
        command_data = 0;
        if (command < serial_interface.handlers_size) {
            bytes_left = serial_interface.handlers[command].data_size;
        }
    } else {
        command_data = command_data << 8;
        command_data = command_data | data;
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

void serial_init(void)
{
    serial_interface.handlers_size = 0;
    serial_interface.handlers = NULL;
    serial_interface.outstanding_data = -1;
}

void serial_set_command_handlers(SerialHandler *array, int size)
{
    serial_interface.handlers = array;
    serial_interface.handlers_size = size;
}

void serial_send(int data)
{
    /* send the high byte first and store the lower one */
    serial_interface.outstanding_data = data & 0xFF;
    data = (data >> 1) & 0xFF;
    bus_write(SERIAL_DATA_ADDR, data);
}

void serial_notify(void)
{
    int status, data;

    status = bus_read(SERIAL_STATUS_ADDR);
    fprintf(stderr, "serial status %d\n", status);

    if (!(status & SERIAL_STATUS_TXBUSY))
    {
      serial_line_free();
    }

    if (status & SERIAL_STATUS_RXAV)
    {
      data = bus_read(SERIAL_DATA_ADDR);
      serial_received_data(data);
    }
}
