#include <unistd.h>
#include <string.h>
#include "swhw_interface.h"

int main(void)
{
    void *addr = (void *)0x1111;

    bus_write(addr, 0x1111);
    bus_read(addr);
    return 0;
}
