# Makefile inspired by Sloan Liu
CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)
OBJS = randstate.o numtheory.o rsa.o

debug: CFLAGS += -g

debug: all

all: keygen encrypt decrypt

keygen: keygen.o $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

encrypt: encrypt.o $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

decrypt: decrypt.o $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f keygen encrypt decrypt *.o

cleankeys:
	rm -f rsa.pub
	rm -f rsa.priv

format:
	clang-format -i -style=file *.[ch]
