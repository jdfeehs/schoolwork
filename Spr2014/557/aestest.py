#!/usr/bin/env python
from Crypto.Cipher import AES
import base64
import binascii
import re
import timeit
def main():
    #this is where the decryption will occur?
    #the partial key is always going to stay the same.
    start = timeit.timeit()
    partialKey = "c0000000000000000000003"
    ciphertext = raw_input()
    #testing stuff
    #
    #
    #
    BLOCK_SIZE = 16 
    PADDING = '{'
    brute_force(partialKey,ciphertext,PADDING)
    end = timeit.timeit()
    #The test case
    key_s = "000000000000000000000000000000FF"
    iv_s = "9876543210FEDCBA9876543210FEDCBA"
    encoded="03D735A237D13DEA619C0E810C6BC262".decode("hex")
    decoded = decrypt(key_s,iv_s,encoded,PADDING)
    print "decoded test case:",decoded
def pad(s,BLOCK_SIZE,PADDING):
    return s + (BLOCK_SIZE - len(s) % BLOCK_SIZE) * PADDING

def decrypt(key_s, iv_s, cipherText,PADDING):
    iv = iv_s.decode("hex")
    key = key_s.decode("hex")
    aes = AES.new(key,AES.MODE_CBC,iv)
    return aes.decrypt(cipherText).rstrip(PADDING)

def encrypt(key_s, iv_s, message,BLOCK_SIZE,PADDING):
    iv = iv_s.decode("hex")
    key = key_s.decode("hex")
    aes = AES.new(key,AES.MODE_CBC,iv)
    return aes.encrypt(pad(message,BLOCK_SIZE,PADDING))
def test_valid(s):
    validChars = set(' ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890`~ @#$%^&*(){}[]+=-_<>,.?/!:\;\'|\"')
    return set(s).issubset(validChars)
def brute_force(partial_key, ciphertext,PADDING):
    iv_s = ciphertext[:32]
    ciphertext = ciphertext[32:]
    for i in xrange(0,68719476736):
        curStr =  hex(i)
        curStr = "" + curStr[2:]
        while(len(curStr) < 9):
            curStr = "0" + curStr
        curStr += partial_key
        #print curStr
        curOut = decrypt(curStr.strip(),iv_s,ciphertext,PADDING)
        if(i % 10000000 == 0):
            print i   
        if test_valid(curOut):
        #if(re.match('[a-zA-z0-9`~@#$%^&*(){}[]+=-_<>,.?/!:\;\'|\"',curOut)):
            print "message:",curOut
            print i
            print "key:",curStr
            sys.exit()

if __name__ == "__main__":
    main()
