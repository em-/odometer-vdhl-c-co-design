#include <string.h>
#include <stdio.h>
#include "simulator_swhw_interface.h"
#include "memory_map.h"

#define DATA_SIZE 2
#define ADDR_SIZE 2

#define DEBUG 0

/*
 * File: swhw_interface.c
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: Software <-> Hardware Interface communicating through stdin/stdout
 */

static irq_handler handlers[DATA_SIZE*8] = {NULL, };
static void *handlers_data[DATA_SIZE*8] = {NULL, };
static int irq_enabled = 1;

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

void set_irq_handler(int line, irq_handler handler, void *data)
{
  handlers[line] = handler;
  handlers_data[line] = data;
}

static void serve_irq()
{
    int line = 0;
    unsigned char masq=1;
    int irq;

    bus(1, 1, IRQ_ADDR, 0, &irq);

    while (masq && !(irq & masq))
    {
        line++;
        masq = masq << 1;
    }

    fprintf(stderr, "software: serving irq %d\n", line);

    irq_enabled = 0;
    if (handlers[line] != NULL)
      handlers[line](handlers_data[line]);
    irq_enabled = 1;
   
    /* Clear the handled IRQ */
    bus(1, 0, IRQ_ADDR, masq, NULL);
}

static int bus(int strobe, int RnW, void *addr, int data, int *dest)
{
    char addr_out[ADDR_SIZE*8+1] = {'\0'};
    char data_out[DATA_SIZE*8+1] = {'\0'};
    char data_in[DATA_SIZE*8] = {'\0'};
    int irq_in;

    char output[256];

    setlinebuf(stdout);

    if (!strobe)
    {
        memset(addr_out, 'Z', ADDR_SIZE*8);
        memset(data_out, 'Z', DATA_SIZE*8);
    }
    else if (RnW)
    {
        to_binary((long)addr, addr_out, ADDR_SIZE);
        memset(data_out, 'Z', DATA_SIZE*8);
    }
    else
    {
        to_binary((long)addr, addr_out, ADDR_SIZE);
        to_binary(data, data_out, DATA_SIZE);
    }

    sprintf(output, "%d %d %s %s", strobe, RnW, addr_out, data_out);
    printf("%s\n", output);

    if(DEBUG)
        fprintf(stderr, "software: -> %s\n", output);

    scanf("%s %d", data_in, &irq_in);

    if(DEBUG)
        fprintf(stderr, "software: <- %s %d\n", data_in, irq_in);

    if (dest != NULL)
    {
        *dest = (int)from_binary(data_in, DATA_SIZE);
    }

    return irq_in;
}

int bus_read(void *address)
{
    int data;
    if (bus(1, 1, address, 0, &data) && irq_enabled)
    {
        serve_irq();
    }
    return data;
}

void bus_write(void *address, int data)
{
    if (bus(1, 0, address, data, NULL) && irq_enabled)
    {
        serve_irq();
    }
}

void bus_sleep()
{
    if (bus(0, 0, NULL, 0, NULL) && irq_enabled)
    {
        serve_irq();
    }
}
