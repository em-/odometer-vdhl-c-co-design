#include <unistd.h>
#include <string.h>
#include "swhw_interface.h"

#define DATA_SIZE 2
#define ADDR_SIZE 2
#define IRQ_SIZE 1

/*
 * File: swhw_interface.c
 * Classe:
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: Software <-> Hardware Interface communicating through stdin/stdout
 */

static irq_handler handlers[IRQ_SIZE*8] = {NULL, };

static void to_binary(long data, char dest[], int size)
{
    int i;
    for (i=size*8-1; i>=0; i--)
    {
        if (data & 1)
        {
            dest[i] = '1';
        }
        else
        {
            dest[i] = '0';
        }

        data = data >> 1;
    }
}

static long from_binary(char src[], int size)
{
    long ret = 0;
    int i;

    for (i=0; i<size*8; i++)
    {
        if (src[i] == '1')
        {
            ret = ret | 1;
        }

        ret = ret << 1;
    }
    return ret;
}

void set_irq_handler(int line, irq_handler handler)
{
  handlers[line] = handler;
}

static void serve_irq(unsigned char irq)
{
    int line = 0;
    unsigned char masq=1;
    char *msg;

    if (irq == 0)
      return;

    while (masq && !(line & masq))
    {
        line++;
        masq = masq << 1;
    }

    msg = "software: serving irq\n";
    write(2, msg, strlen(msg));

    handlers[line]();
}

static int bus(int strobe, int RnW, void *addr, int data)
{
    char addr_out[ADDR_SIZE*8+1] = {'\0'};
    char data_out[DATA_SIZE*8+1] = {'\0'};
    char data_in[DATA_SIZE*8] = {'\0'};
    char irq_in[IRQ_SIZE*8+1] = {'\0'};
    char *msg;

    to_binary((long)addr, addr_out, ADDR_SIZE);

    if (!strobe)
    {
        memset(addr_out, 'Z', ADDR_SIZE*8);
        memset(data_out, 'Z', DATA_SIZE*8);
        write(1, "0 1 ", 4);
        write(1, addr_out, ADDR_SIZE*8);
        write(1, " ", 1);
        write(1, data_out, DATA_SIZE*8);
        write(1, "\n", 1);
    }
    else if (RnW)
    {
        memset(data_out, 'Z', DATA_SIZE*8);
        write(1, "1 1 ", 4);
        write(1, addr_out, ADDR_SIZE*8);
        write(1, " ", 1);
        write(1, data_out, DATA_SIZE*8);
        write(1, "\n", 1);
    }
    else
    {
        to_binary(data, data_out, DATA_SIZE);

        write(1, "1 0 ", 4);
        write(1, addr_out, ADDR_SIZE*8);
        write(1, " ", 1);
        write(1, data_out, DATA_SIZE*8);
        write(1, "\n", 1);
    }

    msg = "software: written\n";
    write(2, msg, strlen(msg));

    if (read(0, data_in, DATA_SIZE*8) < DATA_SIZE*8)
    {
        msg = "software: short read (data)\n";
        write(2, msg, strlen(msg));
    }

    if (read(0, irq_in, IRQ_SIZE*8+1) < IRQ_SIZE*8+1)
    {
        msg = "software: short read (irq)\n";
        write(2, msg, strlen(msg));
    }

    msg = "software: read\n";
    write(2, msg, strlen(msg));

    serve_irq((unsigned char)from_binary(irq_in, IRQ_SIZE));

    return (int)from_binary(data_in, DATA_SIZE);
}

int bus_read(void *address)
{
    return (int)bus(1, 1, address, 0);
}

void bus_write(void *address, int data)
{
    bus(1, 0, address, data);
}

void bus_noop()
{
    bus(0, 0, NULL, 0);
}
