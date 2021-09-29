# File: Makefile

all: main.o readline.o factorial.o
	gcc main.o readline.o factorial.o -o myfactorial
main.o: main.c readline.h factorial.h
	gcc -c main.c
readline.o: readline.c readline.h
	gcc -c readline.c
factorial.o: factorial.c factorial.h
	gcc -c factorial.c
clean:
	rm -f *.o myfactorial
