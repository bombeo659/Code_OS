// findProcessID.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
	printf("Process ID: %d\n", getpid());
	printf("Parent process ID: %d\n", getppid());
	printf("My group: %d\n", getpgrp());

	return 0;
}
