/*
 * File: swhw_interface.h
 * Classe:
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: Software <-> Hardware Interface communicating through stdin/stdout
 */

typedef void (*irq_handler)(void);

void set_irq_handler(int line, irq_handler handler);
int bus_read(void *address);
void bus_write(void *address, int data);
void bus_noop();
