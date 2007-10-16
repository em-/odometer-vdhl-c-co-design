#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "simulator_swhw_interface.h"
#include "serial_interface.h"
#include "odometer.h"
#include "memory_map.h"
#include "util.h"

/*
 * File: serial_interface.c
 * Classe: SerialInterface
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 */

#define SERIAL_STATUS_RXAV   0x1
#define SERIAL_STATUS_TXBUSY 0x2

enum {
  COMMAND_SET_COEFF = 0,
  COMMAND_SET_K,
  COMMAND_SET_K1,
  COMMAND_SET_K2,
  COMMAND_GET_ANGLE,
  COMMAND_GET_REVOLUTIONS
} Commands;

SerialInterface serial_interface;

static void serial_received_data(unsigned char serial_data)
{
    int data;
    if (serial_interface.bytes_left == 0) {
        serial_interface.current_command = serial_data;
        serial_interface.current_command_data = 0;
        switch(serial_interface.current_command) {
            case COMMAND_SET_COEFF:
            case COMMAND_SET_K:
            case COMMAND_SET_K1:
            case COMMAND_SET_K2:
                serial_interface.bytes_left = 2;
                break;
            case COMMAND_GET_ANGLE:
            case COMMAND_GET_REVOLUTIONS:
                serial_interface.bytes_left = 0;
                break;
            default:
                serial_interface.bytes_left = 0; 
        }
    } else {
        data = serial_interface.current_command_data << 8;
        data = data | serial_data;
        serial_interface.current_command_data = data;
        serial_interface.bytes_left--;
    }

    if (serial_interface.bytes_left == 0) {
        switch(serial_interface.current_command) {
            case COMMAND_SET_COEFF:
                odometer_set_coeff(serial_interface.current_command_data);
                break;
            case COMMAND_SET_K:
                odometer_set_K(serial_interface.current_command_data);
                break;
            case COMMAND_SET_K1:
                odometer_set_K1(serial_interface.current_command_data);
                break;
            case COMMAND_SET_K2:
                odometer_set_K2(serial_interface.current_command_data);
                break;
            case COMMAND_GET_ANGLE:
                odometer_get_angle();
                break;
            case COMMAND_GET_REVOLUTIONS:
                odometer_get_revolutions();
                break;
        }
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
    serial_interface.outstanding_data = -1;
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
    DEBUG("serial status %d\n", status);

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
