ex1: main.o ex1.o
	gcc main.o ex1.o -o ex1 -lm
ex1.o: ex1.c ex1.h 
	gcc -c ex1.c -lm
main.o: main.c ex1.h
	gcc -c main.c -lm
clean:
	rm -f *.o ex1
