#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "simulator_swhw_interface.h"
#include "serial_interface.h"
#include "odometer.h"
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
        switch(command) {
            case 0: /* odometer_set_coeff */
            case 1: /* odometer_set_K */
            case 2: /* odometer_set_K1 */
            case 3: /* odometer_set_K2 */
                bytes_left = 2;
                break;
            case 4: /* odometer_get_angle */
            case 5: /* odometer_get_revolutions */
                bytes_left = 0;
                break;
            default:
                bytes_left = 0; 
        }
    } else {
        command_data = command_data << 8;
        command_data = command_data | serial_data;
        bytes_left--;
    }

    if (bytes_left == 0) {
        switch(command) {
            case 0:
                odometer_set_coeff(command_data);
                break;
            case 1:
                odometer_set_K(command_data);
                break;
            case 2:
                odometer_set_K1(command_data);
                break;
            case 3:
                odometer_set_K2(command_data);
                break;
            case 4:
                odometer_get_angle(command_data);
                break;
            case 5:
                odometer_get_revolutions(command_data);
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
