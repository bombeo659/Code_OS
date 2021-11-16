#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 10

//user-defined functions
void * user_def_func(void *threadID){
	long TID;
	TID = (long) threadID;
	printf("Hello World! from thread #%ld\n", TID);
	pthread_exit(NULL);
}

int main(int argc, char *argv){
	pthread_t threads[NUM_THREADS];
	int create_flag;
	long i;
	printf("Example1: Pthread Creation and Termination\n");
	for(i = 0; i < NUM_THREADS; i++){
		printf("In main: creating thread %ld\n", i);
		create_flag = pthread_create(&threads[i], NULL, user_def_func, (void *)i);
		if(create_flag){
			printf("ERROR: return code from pthread_create() is %d\n", create_flag);
			exit(-1);
		}
	}
	// free thread
	pthread_exit(NULL);
	return 0;
}
// gcc ex1.c -o out -lpthread
