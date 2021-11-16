#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 10

/* Thread Argument Passing */
// case-study 1
void * user_def_func(void *threadID){
	long TID;
	TID = (long) threadID;
	printf("Hello World! from thread #%ld\n", TID);
	pthread_exit(NULL);
}

long taskids[NUM_THREADS];
// case-study 2

int main (int argc, char *argv[]){
	pthread_t threads[NUM_THREADS];
	int creation_flag;
	long i;
	printf("Example2: Thread Argument Passing\n");
	for(i = 0; i < NUM_THREADS; i++){
		// pass arguments
		taskids[i] = i;
		printf("In main: creating thread %ld\n", i);
		creation_flag = pthread_create(&threads[i], NULL, user_def_func, (void *)taskids[i]);
		if(creation_flag){
			printf("ERROR: return code from pthread_create() is %d\n", creation_flag);
			exit(-1);
		}
	}
	// free thread
	pthread_exit(NULL);
	return 0;
}
// gcc ex2.c -o out -lpthread
