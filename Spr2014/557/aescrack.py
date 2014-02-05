#!/usr/bin/env python
from Crypto.Cipher import AES
import base64
import binascii
def main():
    #this is where the decryption will occur?
    #the partial key is always going to stay the same.
    partialKey = "C0000000000000000000003"
    #testing stuff
    #
    #
    #
    BLOCK_SIZE = 16 
    PADDING = '{'
    pad = lambda s: s + (BLOCK_SIZE - len(s) % BLOCK_SIZE) * PADDING
    key_s = "000000000000000000000000000000FF"
    iv_s = "9876543210FEDCBA9876543210FEDCBA"
    encoded = encrypt(key_s,iv_s,"Crypto challenge",BLOCK_SIZE,PADDING)
    #print "encoded:",base64.b64encode(encoded)
    decoded = decrypt(key_s,iv_s,encoded,PADDING)
    #print "decoded:",decoded
def pad(s,BLOCK_SIZE,PADDING):
    return s + (BLOCK_SIZE - len(s) % BLOCK_SIZE) * PADDING

def decrypt(key_s, iv_s, cipherText,PADDING):
    iv = base64.b16decode(iv_s)
    key = base64.b16decode(key_s)
    aes = AES.new(key,AES.MODE_CBC,iv)
    return aes.decrypt(cipherText).rstrip(PADDING)

def encrypt(key_s, iv_s, message,BLOCK_SIZE,PADDING):
    iv = base64.b16decode(iv_s)
    key = base64.b16decode(key_s)
    aes = AES.new(key,AES.MODE_CBC,iv)
    return aes.encrypt(pad(message,BLOCK_SIZE,PADDING))



if __name__ == "__main__":
    main()
