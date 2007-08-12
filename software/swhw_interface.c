#include <unistd.h>
#include <string.h>
#include "swhw_interface.h"

#define DATA_SIZE 2
#define ADDR_SIZE 2

#define IRQ_ADDR 0x0000

/*
 * File: swhw_interface.c
 * Classe:
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: Software <-> Hardware Interface communicating through stdin/stdout
 */

static irq_handler handlers[DATA_SIZE*8] = {NULL, };

static int bus(int strobe, int RnW, void *addr, int data, int *dest);

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
        ret = ret << 1;

        if (src[i] == '1')
        {
            ret = ret | 1;
        }
    }
    return ret;
}

void set_irq_handler(int line, irq_handler handler)
{
  handlers[line] = handler;
}

static void serve_irq()
{
    int line = 0;
    unsigned char masq=1;
    char *msg;
    int irq;

    bus(1, 1, IRQ_ADDR, 0, &irq);

    while (masq && !(irq & masq))
    {
        line++;
        masq = masq << 1;
    }

    msg = "software: serving irq\n";
    write(2, msg, strlen(msg));

    if (handlers[line] != NULL)
      handlers[line]();
   
    /* Clear the handled IRQ */
    bus(1, 0, IRQ_ADDR, masq, NULL);
}

static int bus(int strobe, int RnW, void *addr, int data, int *dest)
{
    char *control;
    char addr_out[ADDR_SIZE*8+1] = {'\0'};
    char data_out[DATA_SIZE*8+1] = {'\0'};
    char data_in[DATA_SIZE*8] = {'\0'};
    char irq_in[1+1] = {'\0'};
    char *msg;

    if (!strobe)
    {
        control = "0 1 ";
        memset(addr_out, 'Z', ADDR_SIZE*8);
        memset(data_out, 'Z', DATA_SIZE*8);
    }
    else if (RnW)
    {
        control = "1 1 ";
        to_binary((long)addr, addr_out, ADDR_SIZE);
        memset(data_out, 'Z', DATA_SIZE*8);
    }
    else
    {
        control = "1 0 ";
        to_binary((long)addr, addr_out, ADDR_SIZE);
        to_binary(data, data_out, DATA_SIZE);
    }

    write(1, control, strlen(control));
    write(1, addr_out, ADDR_SIZE*8);
    write(1, " ", 1);
    write(1, data_out, DATA_SIZE*8);
    write(1, "\n", 1);

    write(2, "software: -> ", 13);
    write(2, control, strlen(control));
    write(2, addr_out, ADDR_SIZE*8);
    write(2, " ", 1);
    write(2, data_out, DATA_SIZE*8);
    write(2, "\n", 1);

    if (read(0, data_in, DATA_SIZE*8) < DATA_SIZE*8)
    {
        msg = "software: short read (data)\n";
        write(2, msg, strlen(msg));
    }

    if (read(0, irq_in, 1+1) < 1+1)
    {
        msg = "software: short read (irq)\n";
        write(2, msg, strlen(msg));
    }

    write(2, "software: <-  ", 13);
    write(2, data_in, DATA_SIZE*8);
    write(2, " ", 1);
    write(2, irq_in, 1);
    write(2, "\n", 1);

    if (dest != NULL)
    {
        *dest = (int)from_binary(data_in, DATA_SIZE);
    }

    return irq_in[0] == '1';
}

int bus_read(void *address)
{
    int data;
    if (bus(1, 1, address, 0, &data))
    {
        serve_irq();
    }
    return data;
}

void bus_write(void *address, int data)
{
    if (bus(1, 0, address, data, NULL))
    {
        serve_irq();
    }
}

void bus_noop()
{
    if (bus(0, 0, NULL, 0, NULL))
    {
        serve_irq();
    }
}
