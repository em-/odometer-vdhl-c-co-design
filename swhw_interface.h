/*
 * File: swhw_interface.h
 * Classe:
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: Software <-> Hardware Interface communicating through stdin/stdout
 */

int bus_read(void *address);
void bus_write(void *address, int data);
