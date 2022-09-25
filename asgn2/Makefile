CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
LDFLAGS = -lm

all: mathlib-test

e.o: e.c mathlib.h
	$(CC) $(CFLAGS) -c e.c

newton.o: newton.c mathlib.h
	$(CC) $(CFLAGS) -c newton.c

mathlib-test: mathlib-test.o e.o newton.o euler.o madhava.o bbp.o viete.o
	$(CC) $(LDFLAGS) -o mathlib-test mathlib-test.o e.o newton.o euler.o madhava.o bbp.o viete.o

mathlib-test.o: mathlib-test.c mathlib.h
	$(CC) $(CFLAGS) -c mathlib-test.c

euler.o: euler.c mathlib.h
	$(CC) $(CFLAGS) -c euler.c

madhava.o: madhava.c mathlib.h
	$(CC) $(CFLAGS) -c madhava.c

bbp.o: bbp.c mathlib.h
	$(CC) $(CFLAGS) -c bbp.c

viete.o: viete.c mathlib.h
	$(CC) $(CFLAGS) -c viete.c

clean:
	rm *.o
	rm -f mathlib-test

format:
	clang-format -i -style=file *.[ch]
