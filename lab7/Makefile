all: sched
	@echo "Input file:"
	@cat input.txt
	@echo "Results:"
	@cat input.txt | ./sched
sched:
	gcc sched.c queue.c -o sched -lpthread
clean:
	rm -f *.o sched