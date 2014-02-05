#!/usr/bin/env python
from Crypto.Cipher import AES

def main():
    #this is where the decryption will occur?
    #the partial key is always going to stay the same.
    partialKey = "1100000000000000000000000000000000000000"
    partialKey += "0000000000000000000000000000000000000000000000000011"

