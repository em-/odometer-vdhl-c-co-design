/*
 * File: serial_interface.h
 * Classe:
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 */

typedef struct {
    int data_size;
    void (*handler)(int data);
} SerialHandler;

typedef struct {
    int handlers_size;
    SerialHandler *handlers;
    int outstanding_data;
} SerialInterface;

void serial_init(void);
void serial_set_command_handlers(SerialHandler *array, int size);
void serial_send(int data);
void serial_notify(void);
