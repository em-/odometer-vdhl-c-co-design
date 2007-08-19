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

#define COMMANDS 4

typedef struct {
    int data_size;
    void (*handler)(int data);
} handler;

int coeff, K, K1, K2, angle, revolutions;

void set_coeff(int data) {
    coeff = data;
    fprintf(stderr, "setting coeff: %d\n", coeff);
}

void set_K(int data) {
    K = data;
    fprintf(stderr, "setting K: %d\n", K);
}

void set_K1(int data) {
    K1 = data;
    fprintf(stderr, "setting K1: %d\n", K1);
}

void set_K2(int data) {
    K2 = data;
    fprintf(stderr, "setting K2: %d\n", K2);
}

handler command_handlers[COMMANDS] = {
    {2, set_coeff},
    {2, set_K},
    {2, set_K1},
    {2, set_K2},
};

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
    static int bytes_left = 0;
    static int command, command_data;

    if (bytes_left == 0) {
        command = data;
        command_data = 0;
        if (command < COMMANDS) {
            bytes_left = command_handlers[command].data_size;
        }
    } else {
        command_data = command_data << 8;
        command_data = command_data | data;
        bytes_left--;
    }

    if (bytes_left == 0 && command < COMMANDS) {
        command_handlers[command].handler(command_data);
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
