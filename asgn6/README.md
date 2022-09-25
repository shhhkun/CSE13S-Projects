# Assignment #6: Public Key Cryptography (RSA)
Serjo Barron

sepbarro

Fall 2021

## [files]
 - keygen.c
 - encrypt.c
 - decrypt.c
 - numtheory.h
 - numtheory.c
 - randstate.h
 - randstate.c
 - rsa.h
 - rsa.c
 - Makefile

## [description]
This program implements RSA public cryptography, which means there will be a means to generate a public and private key pair, and then use those keys to both encrypt and decrypt a file. The keys are for the purpose of keeping the action of encrypting exclusive to those who have the public key, this same rule applies for the decryptor, except the receiver must have the private key in order to decrypt the ciphertext to view the original message.

The specific form of cryptography is RSA, which was invented by Ronald Rivest, Adi Shamir, and Leonard Adleman in 1978, hence the reason it is called "RSA". This form of key generation is done by generating 2 large primes p and q, multiplying them to gain an even larger number n. With these 2 numbers, you can then calculate the public and private key pair. The purpose of generating very large prime numbers is to take advantage of the fact that factoring the product is two primes is difficult, and that difficulty only increases as the numbers become larger.

## [instructions]
Once all the files are present (as listed above) in the right directory, compile the executables (keygen, encrypt, decrypt) by running the command "make all". Once done, you can run the executables with the "-h" flag to be prompted with its CLI options, how to use them, and what they do.

example:

./keygen\
./encrypt -i file -o out\
./decrypt -i out -o message

The example above will generate the public and private key pair into some two files, for simplicity we will reference the default rsa.pub and rsa.priv files. Encrypting will then take a file and encrypt it producing ciphertext in out. Encryption will require a public key, and it will take rsa.pub by default. Decrypting will take the ciphertext and decrypt it back to its original contents/state (file), and will be produced in message. Decryption will require a private key, and it will take rsa.priv by default.

More information regarding the I/O of keygen, encrypt, and decrypt, it is available when running the program with "-h" which displays a synopsis message of CLI instructions.

