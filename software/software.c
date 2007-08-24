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

enum Direction {
  DIRECTION_NONE,
  DIRECTION_COUNTERCLOCKWISE,
  DIRECTION_CLOCKWISE
};

typedef struct {
    int coeff;
    int K, K1, K2;
    int angle;
    int revolutions;
    
    int started;
    enum Direction direction;
} Odometer;

Odometer odometer;
SerialInterface serial_interface;

void odometer_init(Odometer *odometer)
{
    odometer->started = 0;
    odometer->direction = DIRECTION_NONE;

    odometer->coeff = odometer->K = odometer->K1 = odometer->K2 =0; 
}

void check_angle(void)
{
    if ((odometer.angle > odometer.K1) 
        && (odometer.angle < odometer.K2)
        && ((odometer.angle % odometer.K) == 0)) {
        fprintf(stderr, "signal\n");
        bus_write((void*)0x4, 0);
    }
}

void set_coeff(int data) {
    odometer.coeff = data;
    fprintf(stderr, "setting coeff: %d\n", odometer.coeff);
}

void set_K(int data) {
    odometer.K = data;
    fprintf(stderr, "setting K: %d\n", odometer.K);
}

void set_K1(int data) {
    odometer.K1 = data;
    fprintf(stderr, "setting K1: %d\n", odometer.K1);
}

void set_K2(int data) {
    odometer.K2 = data;
    fprintf(stderr, "setting K2: %d\n", odometer.K2);
}

void get_angle(int data) {
    serial_send(&serial_interface, odometer.angle);
    fprintf(stderr, "getting angle: %d\n", odometer.angle);
}

void get_revolutions(int data) {
    serial_send(&serial_interface, odometer.revolutions);
    fprintf(stderr, "getting revolutions: %d\n", odometer.revolutions);
}

void encoder_counterclockwise(void *data)
{
    fprintf(stderr, "tick counter-clockwise %d-%d\n", odometer.angle, odometer.coeff);
    if (odometer.started) {
        odometer.angle -= odometer.coeff;
        odometer.direction = DIRECTION_COUNTERCLOCKWISE;
        check_angle();
    }
}

void encoder_clockwise(void *data)
{
    fprintf(stderr, "tick clockwise %d+%d\n", odometer.angle, odometer.coeff);
    if (odometer.started) {
        odometer.angle += odometer.coeff;
        odometer.direction = DIRECTION_CLOCKWISE;
        check_angle();
    }
}

void encoder_revolution(void *data)
{
    if (odometer.coeff != 0)
        odometer.started = 1;
    odometer.angle = 0;
    if (odometer.direction == DIRECTION_COUNTERCLOCKWISE) {
        fprintf(stderr, "full revolution %d-1\n", odometer.revolutions);
        odometer.revolutions--;
    } else if (odometer.direction == DIRECTION_CLOCKWISE) {
        fprintf(stderr, "full revolution %d+1\n", odometer.revolutions);
        odometer.revolutions++;
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

    serial_init(&serial_interface);
    odometer_init(&odometer);

    command_nr = sizeof(command_handlers)/sizeof(SerialHandler);
    serial_set_command_handlers(&serial_interface, command_handlers, command_nr);

    set_irq_handler(0, encoder_counterclockwise, NULL);
    set_irq_handler(1, encoder_clockwise, NULL);
    set_irq_handler(2, encoder_revolution, NULL);
    set_irq_handler(3, serial_notify, &serial_interface);

    for(i=0; i<10000; i++)
      bus_noop();
    return 0;
}
