#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "simulator_swhw_interface.h"
#include "serial_interface.h"
#include "odometer.h"

/*
 * File: software.c
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: top-level software entity
 */

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

    serial_init();
    odometer_init();

    command_nr = sizeof(command_handlers)/sizeof(SerialHandler);
    serial_set_command_handlers(command_handlers, command_nr);

    set_irq_handler(0, odometer_encoder_counterclockwise, NULL);
    set_irq_handler(1, odometer_encoder_clockwise, NULL);
    set_irq_handler(2, odometer_encoder_revolution, NULL);
    set_irq_handler(3, serial_notify, NULL);

    for(i=0; i<10000; i++)
      bus_sleep();
    return 0;
}
