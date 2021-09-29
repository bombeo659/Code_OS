#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> /* defines fork(), and pid_t. */
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

	pid_t child_pid;

	/* lets fork off a child process ... */
	child_pid = fork();

	/*check what the fork() call actually did */
	if(child_pid == -1){
		perror("fork"); /* print a system−defined error message */
		exit (1);
	}
	if (child_pid == 0){
	/*fork () succeeded, we're inside the child process */
		printf("Hello, ");
		fflush (stdout);
	}
       	else {
	/* fork () succeeded, we're inside the parent process */
		wait(NULL);
		printf("World!\n");
		fflush(stdout);
	}
	return 0;
}
