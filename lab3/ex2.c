#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
	int a, b, c, d, f, g, i;
	a = fork();
	if(a == 0) {
		printf("ID of A: %d\n", getppid());
		printf("ID of B: %d\n", getpid());
		b = fork();
		if(b == 0) {
			printf("ID of E: %d\n", getpid());
		}
		else if(b > 0) {
			wait(NULL);
			f = fork();
			if(f == 0) {
				printf("ID of F: %d\n", getpid());
			}
			else if(f > 0) wait(NULL);
		}
	}
	else if(a > 0) {
		wait(NULL);
		c = fork();
		if(c == 0) {
			printf("ID of C: %d\n", getpid());
			g = fork();
			if(g == 0) {
				printf("ID of G: %d\n", getpid());
				i = fork();
				if(i == 0) {
				 	printf("ID of I: %d\n", getpid());
			    }
			    else if(i > 0) wait(NULL);
			}
			else if(g > 0) wait(NULL);
		}
		else if(c > 0) {
			wait(NULL);
			d = fork();
			if(d == 0) { 
				printf("ID of D: %d\n", getpid());
			}
			else if(d > 0) wait(NULL);
		}
	}
	return 0;
}
