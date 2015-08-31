CC=gcc
BINDIR=./bin

all: prog

prog: prog.o crc32.o
	mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/prog prog.o crc32.o

prog.o: crc32.h

clean:
	rm $(BINDIR)/prog
	rm *.o
