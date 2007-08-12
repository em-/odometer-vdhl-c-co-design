#include <unistd.h>
#include <string.h>
#include "swhw_interface.h"

/*
 * File: software.c
 * Classe:
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 */

int main(void)
{
    int i;
    void *addr = (void *)0x1111;

    bus_write(addr, 0x1111);
    bus_read(addr);
    bus_noop();

    for(i=0; i<1500; i++)
      bus_noop();
    return 0;
}
