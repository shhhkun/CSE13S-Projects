# Makefile inspired by Sloan Liu
CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
# OBJS are: bf.o bst.o bv.o ht.o node.o parser.o speck.o
OBJS = bf.o bst.o bv.o ht.o node.o parser.o speck.o

debug: CFLAGS += -g

debug: all

all: banhammer

banhammer: banhammer.o $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f banhammer *.o

format:
	clang-format -i -style=file *.[ch]
