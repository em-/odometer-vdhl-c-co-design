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

void serial_received_data(data)
{
    static serial_wait wait = SERIAL_WAIT_COMMAND;
    static int command, data_1, data_2;

    switch(wait) {
        case SERIAL_WAIT_COMMAND:
            command = data;
            wait = SERIAL_WAIT_DATA_1;
            break;
        case SERIAL_WAIT_DATA_1:
            data_1 = data;
            wait = SERIAL_WAIT_DATA_2;
            break;
        case SERIAL_WAIT_DATA_2:
            data_2 = data;
            wait = SERIAL_WAIT_COMMAND;
            fprintf(stderr, "received command %d: %d %d\n", command, data_1, data_2);
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
    void *addr = (void *)0x1111;

    set_irq_handler(0, encoder_left);
    set_irq_handler(1, encoder_right);
    set_irq_handler(2, encoder_revolution);
    set_irq_handler(3, serial_interrupt);

    bus_write(addr, 0x1111);
    bus_read(addr);
    bus_noop();

    for(i=0; i<1500; i++)
      bus_noop();
    return 0;
}
