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
  DIRECTION_COUNTERCLOCKWISE,
  DIRECTION_CLOCKWISE
} direction = DIRECTION_NONE;

void check_angle(void)
{
    if ((angle > K1) 
        && (angle < K2)
        && ((angle % K) == 0)) {
        fprintf(stderr, "signal\n");
        bus_write((void*)0x4, 0);
    }
}

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

void encoder_counterclockwise(void)
{
    fprintf(stderr, "tick counter-clockwise %d-%d\n", angle, coeff);
    if (started) {
        angle -= coeff;
        direction = DIRECTION_COUNTERCLOCKWISE;
        check_angle();
    }
}

void encoder_clockwise(void)
{
    fprintf(stderr, "tick clockwise %d+%d\n", angle, coeff);
    if (started) {
        angle += coeff;
        direction = DIRECTION_CLOCKWISE;
        check_angle();
    }
}

void encoder_revolution(void)
{
    if (coeff != 0)
        started = 1;
    angle = 0;
    if (direction == DIRECTION_COUNTERCLOCKWISE) {
        fprintf(stderr, "full revolution %d-1\n", revolutions);
        revolutions--;
    } else if (direction == DIRECTION_CLOCKWISE) {
        fprintf(stderr, "full revolution %d+1\n", revolutions);
        revolutions++;
    }
}


int main(void)
{
    int i, command_nr;
    SerialHandler command_handlers[] = {
        {2, set_coeff},
        {2, set_K},
        {2, set_K1},
        {2, set_K2},
        {0, get_angle},
        {0, get_revolutions}
    };

    serial_init();

    coeff = K = K1 = K2 =0; 

    command_nr = sizeof(command_handlers)/sizeof(SerialHandler);
    serial_set_command_handlers(command_handlers, command_nr);

    set_irq_handler(0, encoder_counterclockwise);
    set_irq_handler(1, encoder_clockwise);
    set_irq_handler(2, encoder_revolution);
    set_irq_handler(3, serial_notify);

    for(i=0; i<10000; i++)
      bus_noop();
    return 0;
}
