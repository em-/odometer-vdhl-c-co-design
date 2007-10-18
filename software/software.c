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
    int i;

    /* Initialize the software components */
    serial_init();
    odometer_init();

    /* Register the IRQ handlers */
    set_irq_handler(0, odometer_rotation_counterclockwise);
    set_irq_handler(1, odometer_rotation_clockwise);
    set_irq_handler(2, odometer_revolution);
    set_irq_handler(3, serial_notify);

    /* Main loop, reacting to interrupts */
    for(i=0; i<10000; i++)
      bus_sleep();
    return 0;
}
