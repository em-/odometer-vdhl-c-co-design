#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "swhw_interface.h"
#include "serial_interface.h"

/*
 * File: software.c
 * Classe:
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 */

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


int main(void)
{
    int i, command_nr;
    serial_handler command_handlers[] = {
        {2, set_coeff},
        {2, set_K},
        {2, set_K1},
        {2, set_K2},
    };

    command_nr = sizeof(command_handlers)/sizeof(serial_handler);
    serial_set_command_handlers(command_handlers, command_nr);

    set_irq_handler(0, encoder_left);
    set_irq_handler(1, encoder_right);
    set_irq_handler(2, encoder_revolution);
    set_irq_handler(3, serial_interrupt);

    for(i=0; i<1500; i++)
      bus_noop();
    return 0;
}
