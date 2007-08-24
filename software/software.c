#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "swhw_interface.h"
#include "serial_interface.h"
#include "odometer.h"

/*
 * File: software.c
 * Classe:
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 */

Odometer odometer;
SerialInterface serial_interface;

int main(void)
{
    int i, command_nr;
    SerialHandler command_handlers[] = {
        {2, odometer_set_coeff, &odometer},
        {2, odometer_set_K, &odometer},
        {2, odometer_set_K1, &odometer},
        {2, odometer_set_K2, &odometer},
        {0, odometer_get_angle, &odometer},
        {0, odometer_get_revolutions, &odometer}
    };

    serial_init(&odometer.serial_interface);
    odometer_init(&odometer);

    command_nr = sizeof(command_handlers)/sizeof(SerialHandler);
    serial_set_command_handlers(&odometer.serial_interface, command_handlers, command_nr);

    set_irq_handler(0, odometer_encoder_counterclockwise, &odometer);
    set_irq_handler(1, odometer_encoder_clockwise, &odometer);
    set_irq_handler(2, odometer_encoder_revolution, &odometer);
    set_irq_handler(3, serial_notify, &odometer.serial_interface);

    for(i=0; i<10000; i++)
      bus_noop();
    return 0;
}
