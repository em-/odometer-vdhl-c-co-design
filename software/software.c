#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "swhw_interface.h"

/*
 * File: software.c
 * Classe:
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 */

#define SERIAL_DATA_ADDR   (void*)0x0002
#define SERIAL_STATUS_ADDR (void*)0x0003
#define SERIAL_STATUS_RXAV   0x1
#define SERIAL_STATUS_TXBUSY 0x2

enum _serial_wait {
    SERIAL_WAIT_COMMAND,
    SERIAL_WAIT_DATA_1,
    SERIAL_WAIT_DATA_2
};
typedef enum _serial_wait serial_wait;

enum _command {
    COMMAND_SET_COEFF       = 0,
    COMMAND_SET_K           = 1,
    COMMAND_SET_K1          = 2,
    COMMAND_SET_K2          = 3,
    COMMAND_GET_ANGLE       = 4,
    COMMAND_GET_REVOLUTIONS = 5
};
typedef enum _command command;

void command_received(int command, int data) {
    fprintf(stderr, "received command %d: %d\n", command, data);
}

void encoder_left(void)
{
    fprintf(stderr, "tick left\n"); 
}

void encoder_right(void)
{
    fprintf(stderr, "tick right\n"); 
}

void encoder_revolution(void)
{
    fprintf(stderr, "full revolution\n"); 
}

void serial_received_data(unsigned char data)
{
    static serial_wait wait = SERIAL_WAIT_COMMAND;
    static unsigned char command, data_1;
    int command_data;

    switch(wait) {
        case SERIAL_WAIT_COMMAND:
            command = data;
            switch(command) {
                case COMMAND_SET_COEFF:
                case COMMAND_SET_K:
                case COMMAND_SET_K1:
                case COMMAND_SET_K2:
                    wait = SERIAL_WAIT_DATA_1;
                    break;
                default:
                    wait = SERIAL_WAIT_COMMAND;
                    command_received(command, 0);
            }
            break;
        case SERIAL_WAIT_DATA_1:
            data_1 = data;
            wait = SERIAL_WAIT_DATA_2;
            break;
        case SERIAL_WAIT_DATA_2:
            command_data = data_1;
            command_data = command_data << 8;
            command_data = command_data | data;
            wait = SERIAL_WAIT_COMMAND;
            command_received(command, command_data);
            break;
    }
}

void serial_line_free()
{
    fprintf(stderr, "serial line free\n");
}

void serial_interrupt(void)
{
    int status, data;

    status = bus_read(SERIAL_STATUS_ADDR);
    fprintf(stderr, "serial status %d\n", status);

    if (status & SERIAL_STATUS_RXAV)
    {
      data = bus_read(SERIAL_DATA_ADDR);
      serial_received_data(data);
    }

    if (!(status & SERIAL_STATUS_TXBUSY))
    {
      serial_line_free();
    }
}


int main(void)
{
    int i;

    set_irq_handler(0, encoder_left);
    set_irq_handler(1, encoder_right);
    set_irq_handler(2, encoder_revolution);
    set_irq_handler(3, serial_interrupt);

    for(i=0; i<1500; i++)
      bus_noop();
    return 0;
}
