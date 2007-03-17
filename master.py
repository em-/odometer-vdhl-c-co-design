#!/usr/bin/python

import sys, time
import binary

def bus(RnW, addr, data=None):
    addr_size = 2
    data_size = 2

    addr_out = binary.to_bin(addr, addr_size)

    if data:
        data_out = binary.to_bin(data, data_size)
    else:
        data_out = 'Z' * data_size * 8
    
    message = "%s %s %s\n" % (RnW, addr_out, data_out)
    sys.stdout.write(message)
    sys.stdout.flush()
    
    data_in = sys.stdin.read(data_size*8 + 1)

    sys.stderr.write('master got %s\n' % data_in)
    return data_in

def write(addr, data):
    bus(0, addr, data)

def read(addr):
    return bus(1, addr)

if __name__ == '__main__':
    sys.stderr.write('master writing\n')
    time.sleep(1)
    write(0x00, 0x01)
    sys.stderr.write('master written\n')
    time.sleep(1)
    sys.stderr.write('master reading\n')
    r = read(0x00)
    sys.stderr.write('master read\n')
    time.sleep(1)
