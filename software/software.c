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

int started = 0;
enum Direction {
  DIRECTION_NONE,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
} direction = DIRECTION_NONE;

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

void get_angle(int data) {
    serial_send(angle);
    fprintf(stderr, "getting angle: %d\n", angle);
}

void get_revolutions(int data) {
    serial_send(revolutions);
    fprintf(stderr, "getting revolutions: %d\n", revolutions);
}

void encoder_left(void)
{
    fprintf(stderr, "tick left %d-%d\n", angle, coeff);
    if (started) {
        angle -= coeff;
        direction = DIRECTION_LEFT;
    }
}

void encoder_right(void)
{
    fprintf(stderr, "tick right %d+%d\n", angle, coeff);
    if (started) {
        angle += coeff;
        direction = DIRECTION_RIGHT;
    }
}

void encoder_revolution(void)
{
    if (coeff != 0)
        started = 1;
    angle = 0;
    if (direction == DIRECTION_LEFT) {
        fprintf(stderr, "full revolution %d-1\n", revolutions);
        revolutions--;
    } else if (direction == DIRECTION_RIGHT) {
        fprintf(stderr, "full revolution %d+1\n", revolutions);
        revolutions++;
    }
}


int main(void)
{
    int i, command_nr;
    serial_handler command_handlers[] = {
        {2, set_coeff},
        {2, set_K},
        {2, set_K1},
        {2, set_K2},
        {0, get_angle},
        {0, get_revolutions}
    };

    coeff = K = K1 = K2 =0; 

    command_nr = sizeof(command_handlers)/sizeof(serial_handler);
    serial_set_command_handlers(command_handlers, command_nr);

    set_irq_handler(0, encoder_left);
    set_irq_handler(1, encoder_right);
    set_irq_handler(2, encoder_revolution);
    set_irq_handler(3, serial_interrupt);

    for(i=0; i<10000; i++)
      bus_noop();
    return 0;
}
