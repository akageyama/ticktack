.PHONY: runme clean

CC = gcc -O0 # -g -traceback -fp-trap-all=overflow

run: runme
	./runme

runme: main.o ticktack.o
	$(CC) -o runme main.o ticktack.o

ticktack.o: ticktack.c
	$(CC) -c ticktack.c

main.o: main.c ticktack.h
	$(CC) -c main.c

clean:
	rm -rf *.o runme
