#!/usr/bin/python

hex_digits = {
    '0':'0000',
    '1':'0001',
    '2':'0010',
    '3':'0011',
    '4':'0100',
    '5':'0101',
    '6':'0110',
    '7':'0111',
    '8':'1000',
    '9':'1001',
    'a':'1010',
    'b':'1011',
    'c':'1100',
    'd':'1101',
    'e':'1110',
    'f':'1111'
}

def to_bin(n, bytes):
    h = hex(n)[2:]

    while len(h) < bytes*2:
        h = "0" + h
    
    str = ""
    for digit in h:
        str += hex_digits[digit]
    return str

def from_bin(s):
    r = 0
    s = s[::-1]
    for place in range(len(s)):
        if s[place] == '1':
            r += 2**place
    return r

def two_complement(n, bytes):
    if n < 0:
        # This is 2**bit - abs(n)
        n = 2**(8*bytes) + n
    return to_bin(n, bytes)

def sign_magnitude(n, bytes):
    if n < 0:
        n = -n
        str = to_bin(n, bytes)
        str = "1" + str[1:]
    else:
        str = to_bin(n, bytes)
    return str


def main(args):
    for i in args[1:]:
        print two_complement(int(i), 1)
        print sign_magnitude(int(i), 1)

if __name__ == "__main__":
    import sys
    main(sys.argv)
