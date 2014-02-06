#!/usr/bin/env python
from Crypto.Cipher import AES
import base64
import binascii
import re
def main():
    #this is where the decryption will occur?
    #the partial key is always going to stay the same.
    partialKey = "c0000000000000000000003"
    ciphertext = raw_input()
    #testing stuff
    #
    #
    #
 
    BLOCK_SIZE = 16 
    PADDING = '{'
    brute_force(partialKey,ciphertext,PADDING)
    key_s = "000000000000000000000000000000FF"
    iv_s = "9876543210FEDCBA9876543210FEDCBA"
    encoded = encrypt(key_s,iv_s,"Crypto challenge",BLOCK_SIZE,PADDING)
    #print "encoded:",base64.b64encode(encoded)
    decoded = decrypt(key_s,iv_s,encoded,PADDING)
    #print "decoded:",decoded
def pad(s,BLOCK_SIZE,PADDING):
    return s + (BLOCK_SIZE - len(s) % BLOCK_SIZE) * PADDING

def decrypt(key_s, iv_s, cipherText,PADDING):
    #iv = base64.b16decode(iv_s)
    iv = iv_s.decode("hex")
    key = key_s.decode("hex")
    #key = base64.b16decode(key_s)
    aes = AES.new(key,AES.MODE_CBC,iv)
    return aes.decrypt(cipherText).rstrip(PADDING)

def encrypt(key_s, iv_s, message,BLOCK_SIZE,PADDING):
    #iv = base64.b16decode(iv_s)
    iv = iv_s.decode("hex")
    key = key_s.decode("hex")
    #key = base64.b16decode(key_s)
    aes = AES.new(key,AES.MODE_CBC,iv)
    return aes.encrypt(pad(message,BLOCK_SIZE,PADDING))

def brute_force(partial_key, ciphertext,PADDING):
    iv_s = ciphertext[:32]
    ciphertext = ciphertext[32:]
    #print "iv:" , iv_s
    #print "ciphertext:", ciphertext
    #print partial_key
    validChars = set('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890`~@#$%^&*(){}[]+=-_<>,.?/!:\;\'|\"')
    for i in xrange(100):  #(68719476736):
        curStr =  hex(i)
        curStr = "" + curStr[2:]
        while(len(curStr) < 9):
            curStr = "0" + curStr
        curStr += partial_key
        #print curStr
        curOut = decrypt(curStr.strip(),iv_s,ciphertext,PADDING)
        #if curOut.issubset(validChars):
        if(re.match('[a-zA-z0-9`~@#$%^&*(){}[]+=-_<>,.?/!:\;\'|\"',curOut)):
            print curOut
if __name__ == "__main__":
    main()
