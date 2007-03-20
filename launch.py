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

def main():
    m2s = os.pipe()
    s2m = os.pipe()

    pid = os.fork()
    if pid != 0:
        os.dup2(s2m[0], 0)
        os.dup2(m2s[1], 1)
        sys.stderr.write('software starting\n')
        os.execl('./software')
    else:
        time.sleep(2)
        os.dup2(m2s[0], 0)
        os.dup2(s2m[1], 1)
        sys.stderr.write('hardware starting\n')
        os.execl('./hardware')

if __name__ == '__main__':
    main()
