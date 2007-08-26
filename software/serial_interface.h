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
    int serial_data_size;
    void (*handler)(int serial_data, void *data);
    void *data;
} SerialHandler;

typedef struct {
    int handlers_size;
    SerialHandler *handlers;
    int outstanding_data;
} SerialInterface;

void serial_init(SerialInterface *serial_interface);
void serial_set_command_handlers(SerialInterface *serial_interface, SerialHandler *array, int size);
void serial_send(SerialInterface *serial_interface, int serial_data);
void serial_notify(void *data);

#endif /* SERIAL_INTERFACE_H */
