#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "simulator_swhw_interface.h"
#include "serial_interface.h"
#include "odometer.h"
#include "memory_map.h"
#include "util.h"

/*
 * File: odometer.c
 * Classe: Odometer
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 */

Odometer odometer;

void odometer_init()
{
    odometer.started = 0;
    odometer.direction = DIRECTION_NONE;

    odometer.coeff = odometer.K = odometer.K1 = odometer.K2 =0; 
}

/* Check if the angle satisfies the params and request
 * an output tick accordingly */
static void odometer_check_angle()
{
    if ((odometer.angle > odometer.K1) 
        && (odometer.angle < odometer.K2)
        && ((odometer.angle % odometer.K) == 0)) {
        DEBUG("request tick\n");
        bus_write(OUTPUT_ADDR, 0);
    }
}

void odometer_set_coeff(int command_data) {
    odometer.coeff = command_data;
    DEBUG("setting coeff: %d\n", odometer.coeff);
}

void odometer_set_K(int command_data) {
    odometer.K = command_data;
    DEBUG("setting K: %d\n", odometer.K);
}

void odometer_set_K1(int command_data) {
    odometer.K1 = command_data;
    DEBUG("setting K1: %d\n", odometer.K1);
}

void odometer_set_K2(int command_data) {
    odometer.K2 = command_data;
    DEBUG("setting K2: %d\n", odometer.K2);
}

void odometer_get_angle() {
    serial_send(odometer.angle);
    DEBUG("getting angle: %d\n", odometer.angle);
}

void odometer_get_revolutions() {
    serial_send(odometer.revolutions);
    DEBUG("getting revolutions: %d\n", odometer.revolutions);
}

void odometer_rotation_counterclockwise()
{
    DEBUG("rotated counter-clockwise %d-%d = ",
        odometer.angle, odometer.coeff);
    if (odometer.started) {
        odometer.angle -= odometer.coeff;
        odometer.direction = DIRECTION_COUNTERCLOCKWISE;
        DEBUG("%d\n", odometer.angle);
        odometer_check_angle(odometer);
    }
    else
    {
        DEBUG("0 (not yet started)\n");
    }
}

void odometer_rotation_clockwise()
{
    DEBUG("rotated clockwise %d+%d = ",
        odometer.angle, odometer.coeff);
    if (odometer.started) {
        odometer.angle += odometer.coeff;
        odometer.direction = DIRECTION_CLOCKWISE;
        DEBUG("%d\n", odometer.angle);
        odometer_check_angle(odometer);
    }
    else
    {
        DEBUG("0 (not yet started)\n");
    }
}

void odometer_revolution()
{
    if (odometer.coeff != 0)
        odometer.started = 1;
    DEBUG("full revolution %d", odometer.revolutions);
    if (odometer.direction == DIRECTION_COUNTERCLOCKWISE) {
        DEBUG("-1");
        odometer.revolutions--;
    } else if (odometer.direction == DIRECTION_CLOCKWISE) {
        DEBUG("+1");
        odometer.revolutions++;
    }
    DEBUG(" = %d", odometer.revolutions);
    odometer.angle = 0;
    DEBUG(" (resetting angle to %d)\n", odometer.angle);
}
