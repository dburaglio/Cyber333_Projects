from pwn import *
import string

r = remote('127.0.0.1', 4444)
r.send(cyclic(120, alphabet = string.printable))

