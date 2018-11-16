CC=gcc -g -Wall
CFLAGS=-I.
DEPS=acsh.h structs.h
OBJ=main.o acsh.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

customshell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o customshell
