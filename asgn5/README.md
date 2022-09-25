# Assignment #5: Huffman Coding
Serjo Barron

sepbarro

Fall 2021

## [files]
 - encode.c
 - decode.c
 - defines.h
 - header.h
 - node.h
 - node.c
 - pq.h
 - pq.c
 - code.h
 - code.c
 - io.h
 - io.c
 - stack.h
 - stack.c
 - huffman.h
 - huffman.c
 - Makefile

## [description]
This program implements a form of data compression known as "Huffman Coding". There are 2 main executable files or Test Harnesses, which will be encode.c (the encoder) and decode.c (the decoder). Together, these files can encode any file and compress its data, and take that compressed file and uncompress it to retrieve the original contents and size of the file. It should be noted however that this method of data compression is not always guaranteeed to save space/reduce the file size (in bytes). There are things to consider before deciding whether compressing a certain file would truly be efficient with Huffman Coding. Encoding a file including the word "banana" for instance would infact increase the size of the encoded file, not decrease it, so in this case the data compression is quite inefficient. However, when dealing with more practical and larger files the effects of data compression using Huffman Coding as drastically more noticeable and far more efficient.

The encoder and decoder will be making use of various ADT's, 6 to be exact, which is the node, priority queue, code, I/O, stack, and huffman ADT's. Each of these ADT's implement the functions needed to perform data compression and the files including these functions will all be linked when compiling the encoder/decoder. As such, encode.c and decode.c will include the main source of user-interaction, where the user can decide whether they want to the encoder/decoder to read from stdin or stdout, or if they want to create a file themselves which contains the message to be decoded and set the encoded message to be written to a specified outfile.

## [instructions]
Once all the required files (as listed above) are present in the right directory, you can compile the program to create the executables encode and decode with "make". First lets discuss encode, if specified with "-i", encode will read the contents of the file and write its encoded version to outfile if specified with "-o". If desired, the user may also toggle "-v" to print statistics which include the size of the file before compression, and after compression in bytes. A space saving statistic will also be computed to determine whether the encoding of this file compressed data. If the space saving is above 100%, then the data compression did not work for that particular file, it instead may have not changed the file size or may have even increased the file size when encoding. For more information run "./encode -h" to print a synopsis message containing the specific usage of the CLI (command line options).

Similar to encode, decode will also take an infile if specified with "-i", the infile should be the produced outfile of encode and include a properly created header. The header includes a magic number indicating that a file is valid to be decoded, the file size, the tree size, and the file permissions that will be used to set the outfile perms. The decoded message will be written to outfile if specified with "-o". And just like encode, the user may toggle "-v" to print statistics including the compressed, decompressd, and space saving of the decoded message. If the decoding went successful, these statistics should match up with the statistics produced when encoding the file in the first place. For more information of all the CLI options, run "./decode -h".
