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

void odometer_check_angle(void)
{
    if ((odometer.angle > odometer.K1) 
        && (odometer.angle < odometer.K2)
        && ((odometer.angle % odometer.K) == 0)) {
        fprintf(stderr, "signal\n");
        bus_write((void*)0x4, 0);
    }
}

void odometer_set_coeff(int command_data, void *data) {
    odometer.coeff = command_data;
    fprintf(stderr, "setting coeff: %d\n", odometer.coeff);
}

void odometer_set_K(int command_data, void *data) {
    odometer.K = command_data;
    fprintf(stderr, "setting K: %d\n", odometer.K);
}

void odometer_set_K1(int command_data, void *data) {
    odometer.K1 = command_data;
    fprintf(stderr, "setting K1: %d\n", odometer.K1);
}

void odometer_set_K2(int command_data, void *data) {
    odometer.K2 = command_data;
    fprintf(stderr, "setting K2: %d\n", odometer.K2);
}

void odometer_get_angle(int command_data, void *data) {
    serial_send(&serial_interface, odometer.angle);
    fprintf(stderr, "getting angle: %d\n", odometer.angle);
}

void odometer_get_revolutions(int command_data, void *data) {
    serial_send(&serial_interface, odometer.revolutions);
    fprintf(stderr, "getting revolutions: %d\n", odometer.revolutions);
}

void odometer_encoder_counterclockwise(void *data)
{
    fprintf(stderr, "tick counter-clockwise %d-%d\n", odometer.angle, odometer.coeff);
    if (odometer.started) {
        odometer.angle -= odometer.coeff;
        odometer.direction = DIRECTION_COUNTERCLOCKWISE;
        odometer_check_angle();
    }
}

void odometer_encoder_clockwise(void *data)
{
    fprintf(stderr, "tick clockwise %d+%d\n", odometer.angle, odometer.coeff);
    if (odometer.started) {
        odometer.angle += odometer.coeff;
        odometer.direction = DIRECTION_CLOCKWISE;
        odometer_check_angle();
    }
}

void odometer_encoder_revolution(void *data)
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
        {2, odometer_set_coeff, NULL},
        {2, odometer_set_K, NULL},
        {2, odometer_set_K1, NULL},
        {2, odometer_set_K2, NULL},
        {0, odometer_get_angle, NULL},
        {0, odometer_get_revolutions, NULL}
    };

    serial_init(&serial_interface);
    odometer_init(&odometer);

    command_nr = sizeof(command_handlers)/sizeof(SerialHandler);
    serial_set_command_handlers(&serial_interface, command_handlers, command_nr);

    set_irq_handler(0, odometer_encoder_counterclockwise, NULL);
    set_irq_handler(1, odometer_encoder_clockwise, NULL);
    set_irq_handler(2, odometer_encoder_revolution, NULL);
    set_irq_handler(3, serial_notify, &serial_interface);

    for(i=0; i<10000; i++)
      bus_noop();
    return 0;
}
