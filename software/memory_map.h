/*
 * File: memory_map.h
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: define the addresses of the hardware components
 */

#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#define IRQ_ADDR           (void*)0x1000
#define SERIAL_DATA_ADDR   (void*)0x1001
#define SERIAL_STATUS_ADDR (void*)0x1002
#define OUTPUT_ADDR        (void*)0x1003

#endif /* MEMORY_MAP_H */
