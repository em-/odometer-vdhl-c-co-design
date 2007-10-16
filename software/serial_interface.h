/*
 * File: serial_interface.h
 * Classe: SerialInterface
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: handle commands from the serial line
 */

#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

typedef enum {
  COMMAND_SET_COEFF = 0,
  COMMAND_SET_K,
  COMMAND_SET_K1,
  COMMAND_SET_K2,
  COMMAND_GET_ANGLE,
  COMMAND_GET_REVOLUTIONS
} Command;

typedef struct {
    int outstanding_data;
    int bytes_left;
    Command current_command;
    int current_command_data;
} SerialInterface;

void serial_init();
void serial_send(int serial_data);
void serial_notify(void *data);

#endif /* SERIAL_INTERFACE_H */
