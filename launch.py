#!/usr/bin/python
# launch.py
#
# Launch two processes communicating with each other on their 
# respective stdin/stdout
#
# Copyright Emanuele Aina <em@nerd.ocracy.org>, 2007.
# Released under the terms of the LGPL

from subprocess import Popen, PIPE

A = './software/software'
B = './hardware/hardware'

def main():
    a = Popen([A], stdin=PIPE, stdout=PIPE)
    print "Started %s" % A
    b = Popen([B], stdin=a.stdout, stdout=a.stdin)
    print "Started %s" % B

    print "Waiting %s" % A
    a.wait()
    print "Waiting %s" % B
    b.wait()

if __name__ == '__main__':
    main()
