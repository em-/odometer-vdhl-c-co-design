/*
 * File: serial_interface.h
 * Classe: SerialInterface
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: handle commands from the serial line
 */

#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

typedef struct {
    int outstanding_data;
} SerialInterface;

void serial_init();
void serial_send(int serial_data);
void serial_notify(void *data);

#endif /* SERIAL_INTERFACE_H */
