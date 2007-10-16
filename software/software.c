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

    set_irq_handler(0, odometer_encoder_counterclockwise, NULL);
    set_irq_handler(1, odometer_encoder_clockwise, NULL);
    set_irq_handler(2, odometer_encoder_revolution, NULL);
    set_irq_handler(3, serial_notify, NULL);

    for(i=0; i<10000; i++)
      bus_sleep();
    return 0;
}
