#!/usr/bin/env python
# -*- coding: utf-8 -*-

from struct import unpack, pack
from binascii import hexlify
from ctypes import *
from ctypes import cdll


codec = cdll.LoadLibrary('./libcodec.so')


fin = open("vmlinux", "rb")
fout = open("encoded_vmlinux", "wb")
BLOCKSIZE = 4096

while True:
    data = fin.read(BLOCKSIZE)
    if len(data) == 0:
        break
    else:
        for byte in data:
            uint16_t = codec.encode_8b10b(byte)
            dataout = pack('H', uint16_t)
            fout.write(dataout)

