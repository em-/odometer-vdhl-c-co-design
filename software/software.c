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

int i;

int main(void)
{
    serial_init();
    odometer_init();

    set_irq_handler(0, odometer_rotation_counterclockwise);
    set_irq_handler(1, odometer_rotation_clockwise);
    set_irq_handler(2, odometer_revolution);
    set_irq_handler(3, serial_notify);

    for(i=0; i<10000; i++)
      bus_sleep();
    return 0;
}
