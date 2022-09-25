CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
LDFLAGS = -lm

all: sorting

stats.o: stats.c stats.h
	$(CC) $(CFLAGS) -c stats.c

insert.o: insert.c insert.h stats.h
	$(CC) $(CFLAGS) -c insert.c

shell.o: shell.c shell.h stats.h
	$(CC) $(CFLAGS) -c shell.c

heap.o: heap.c heap.h stats.h
	$(CC) $(CFLAGS) -c heap.c

quick.o: quick.c quick.h stats.h
	$(CC) $(CFLAGS) -c quick.c

sorting: sorting.o stats.o insert.o shell.o heap.o quick.o
	$(CC) $(LDFLAGS) -o sorting sorting.o insert.o stats.o shell.o heap.o quick.o

sorting.o: sorting.c set.h insert.h shell.h heap.h quick.h stats.h
	$(CC) $(CFLAGS) -c sorting.c

clean:
	rm *.o
	rm -f sorting

format:
	clang-format -i -style=file *.[ch]
