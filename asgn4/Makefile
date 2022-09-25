CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
LDFLAGS = -lm

all: tsp

graph.o: graph.c graph.h vertices.h
	$(CC) $(CFLAGS) -c graph.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

path.o: path.c path.h graph.h stack.h vertices.h
	$(CC) $(CFLAGS) -c path.c

tsp: tsp.o graph.o stack.o path.o
	$(CC) $(LDFLAGS) -o tsp tsp.o graph.o stack.o path.o

tsp.o: tsp.c graph.h stack.h path.h vertices.h
	$(CC) $(CFLAGS) -c tsp.c

clean:
	rm *.o
	rm -f tsp

format:
	clang-format -i -style=file *.[ch]
