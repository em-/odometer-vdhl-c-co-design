#!/usr/bin/python
# launch.py
#
# Launch two processes communicating with each other on their 
# respective stdin/stdout
#
# Copyright Emanuele Aina <em@nerd.ocracy.org>, 2007.
# Released under the terms of the LGPL

import os, sys, time
from subprocess import Popen, PIPE

a = './software'
b = './hardware'

def main():
    a2b = os.pipe()
    b2a = os.pipe()

    pid = os.fork()
    if pid != 0:
        os.dup2(b2a[0], 0)
        os.dup2(a2b[1], 1)
        sys.stderr.write(a + ' starting\n')
        os.execl(a)
    else:
        time.sleep(2)
        os.dup2(a2b[0], 0)
        os.dup2(b2a[1], 1)
        sys.stderr.write(b + ' starting\n')
        os.execl(b)

if __name__ == '__main__':
    main()
