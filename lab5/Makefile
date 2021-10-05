pi_serial: pi_serial.o
	gcc pi_serial.o -o pi_serial -lm -pthread
pi_serial.o:
	gcc -c pi_serial.c -lm -pthread

pi_multi-thread: pi_multi-thread.o
	gcc pi_multi-thread.o -o pi_multi-thread -lm -pthread
pi_multi-thread.o:
	gcc -c pi_multi-thread.c -lm -pthread

code: code.o
	gcc code.o -o code -lm -pthread
code.o:
	gcc -c code.c -lm -pthread

clean:
	rm -f *.o code pi_serial pi_multi-thread
