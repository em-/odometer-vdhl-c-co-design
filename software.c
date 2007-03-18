#include <unistd.h>
#include <string.h>

#define DATA_SIZE 2
#define ADDR_SIZE 2

void to_binary(long data, char dest[], int size)
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

long from_binary(char src[], int size)
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

int bus(int RnW, void *addr, int data)
{
    char addr_out[ADDR_SIZE*8+1] = {'\0'};
    char data_out[DATA_SIZE*8+1] = {'\0'};
    char data_in[DATA_SIZE*8+1] = {'\0'};
    int i;
    char *msg;

    to_binary((long)addr, addr_out, ADDR_SIZE);

    if (RnW)
    {
        write(1, "1 ", 2);
        write(1, addr_out, ADDR_SIZE*8);
        write(1, " ", 1);
        for (i=0; i<DATA_SIZE; i++)
            write(1, "ZZZZZZZZ", 8);
        write(1, "\n", 1);
    }
    else
    {
        to_binary(data, data_out, DATA_SIZE);

        write(1, "0 ", 2);
        write(1, addr_out, ADDR_SIZE*8);
        write(1, " ", 1);
        write(1, data_out, DATA_SIZE*8);
        write(1, "\n", 1);
    }

    msg = "software: written\n";
    write(2, msg, strlen(msg));

    if (read(0, data_in, DATA_SIZE*8+1) < DATA_SIZE*8+1)
    {
        msg = "software: short read\n";
        write(2, msg, strlen(msg));
    }

    msg = "software: read\n";
    write(2, msg, strlen(msg));
    return (int)from_binary(data_in, DATA_SIZE);
}

int bus_read(void *address)
{
    return (int)bus(1, address, 0);
}

void bus_write(void *address, int data)
{
    bus(0, address, data);
}

int main(void)
{
    void *addr = (void *)0x1111;

    bus_write(addr, 0x1111);
    bus_read(addr);
    return 0;
}
