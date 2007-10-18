/*
 * File: swhw_interface.h
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: Software <-> Hardware Interface communicating through stdin/stdout
 */

#ifndef SWHW_INTERFACE_H
#define SWHW_INTERFACE_H

typedef void (*irq_handler)();

/* Register the handler for the IRQ 'line' */
void set_irq_handler(int line, irq_handler handler);

/* Emulate a read on the bus address 'address' */
int bus_read(void *address);

/* Emulate a write to the bus address 'address' */
void bus_write(void *address, int data);

/* Sit idle on the bus servicing IRQs (if any) */
void bus_sleep();

#endif /* SWHW_INTERFACE_H */
