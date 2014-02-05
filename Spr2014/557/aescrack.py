#!/usr/bin/env python
from Crypto.Cipher import AES
import base64
import binascii
def main():
    #this is where the decryption will occur?
    #the partial key is always going to stay the same.
    partialKey = "1100000000000000000000000000000000000000"
    partialKey += "0000000000000000000000000000000000000000000000000011"
    #testing stuff
    #
    #
    #
    BLOCK_SIZE = 16 
    PADDING = '{'
    pad = lambda s: s + (BLOCK_SIZE - len(s) % BLOCK_SIZE) * PADDING
    key_s = "000000000000000000000000000000FF"
    iv_s = "9876543210FEDCBA9876543210FEDCBA"
    iv = base64.b16decode(iv_s)
    key = base64.b16decode(key_s)
    aes = AES.new(key,AES.MODE_CBC,iv)
   
    #encoded = base64.b64encode(aes.encrypt(pad("Crypto challenge")))
    encoded = aes.encrypt(pad("Crypto challenge"))
    print "encoded:",base64.b64encode(encoded)
    #decoded = aes.decrypt(base64.b64decode(encoded))
    aes2 = AES.new(key,AES.MODE_CBC,iv)
    decoded = aes2.decrypt(encoded).rstrip(PADDING)
    print "decoded:",decoded

if __name__ == "__main__":
    main()
