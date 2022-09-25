CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
LDFLAGS = -lm

debug: CFLAGS += -g

debug: all

all: encode decode

io.o: io.c io.h code.h defines.h
	$(CC) $(CFLAGS) -c io.c

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

pq.o: pq.c pq.h node.h
	$(CC) $(CFLAGS) -c pq.c

code.o: code.c code.h defines.h
	$(CC) $(CFLAGS) -c code.c

stack.o: stack.c stack.h node.h
	$(CC) $(CFLAGS) -c stack.c

huffman.o: huffman.c huffman.h node.h code.h pq.h io.h stack.h defines.h
	$(CC) $(CFLAGS) -c huffman.c

encode: encode.o io.o node.o pq.o code.o huffman.o stack.o
	$(CC) $(LDFLAGS) -o encode encode.o io.o node.o pq.o code.o huffman.o stack.o

encode.o: encode.c io.h node.h pq.h code.h huffman.h defines.h header.h
	$(CC) $(CFLAGS) -c encode.c

decode: decode.o io.o node.o pq.o code.o huffman.o stack.o
	$(CC) $(LDFLAGS) -o decode decode.o io.o node.o pq.o code.o huffman.o stack.o

decode.o: decode.c io.h node.h pq.h code.h huffman.h defines.h header.h
	$(CC) $(CFLAGS) -c decode.c

clean:
	rm *.o
	rm -f encode
	rm -f decode

format:
	clang-format -i -style=file *.[ch]
