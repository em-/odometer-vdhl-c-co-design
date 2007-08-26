/*
 * File: swhw_interface.h
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: Software <-> Hardware Interface communicating through stdin/stdout
 */

#ifndef SWHW_INTERFACE_H
#define SWHW_INTERFACE_H

typedef void (*irq_handler)(void *data);

void set_irq_handler(int line, irq_handler handler, void *data);
int bus_read(void *address);
void bus_write(void *address, int data);
void bus_noop();

#endif /* SWHW_INTERFACE_H */
