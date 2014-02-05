from Crypto.Cipher import AES
import base64
import os
import binascii 
# the block size for the cipher object; must be 16, 24, or 32 for AES
BLOCK_SIZE = 16 
 
# the character used for padding--with a block cipher such as AES, the value
# you encrypt must be a multiple of BLOCK_SIZE in length.  This character is
# used to ensure that your value is always a multiple of BLOCK_SIZE
PADDING = '{'
 
# one-liner to sufficiently pad the text to be encrypted
pad = lambda s: s + (BLOCK_SIZE - len(s) % BLOCK_SIZE) * PADDING
 
# one-liners to encrypt/encode and decrypt/decode a string
# encrypt with AES, encode with base64
EncodeAES = lambda c, s: base64.b64encode(c.encrypt(pad(s)))
DecodeAES = lambda c, e: c.decrypt(base64.b64decode(e)).rstrip(PADDING)
 
# generate a random secret key
#secret = os.urandom(BLOCK_SIZE)
secret = "000000000000000000000000000000FF".decode("hex")
print len(secret)
iv = "9876543210FEDCBA9876543210FEDCBA".decode("hex")
print len(iv), "     :",iv.encode("hex")

#iv = 16 * '\x00'
#iv = os.urandom(16)
# create a cipher object using the random secret
cipher = AES.new(secret,AES.MODE_CBC,iv)
#cipher = AES.new(secret)
# encode a string
encoded = EncodeAES(cipher, b"Crypto challenge")
print 'Encrypted string:', encoded
cipher2 = AES.new(secret,AES.MODE_CBC,iv)
# decode the encoded string
decoded = DecodeAES(cipher2, encoded)
print 'Decrypted string:', decoded
