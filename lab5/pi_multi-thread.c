#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_THREADS 100 //number of threads
#define SEED 35791246
int iterationChild;

void *runner(void *tid)
{
    double x, y;
    int *counter = (int *)malloc(sizeof(int));
    *counter = 0;

    srand(SEED);

    for (int i = 0; i < iterationChild; i++)
    {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        if ((x * x + y * y) <= 1) // nam trong vong trong ban kinh 1
            *counter += 1;
    }
    pthread_exit((void *)counter);
    free(counter);
    return NULL;
}

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage : a.out <integer_value>\n");
        return -1;
    }
    if (atoi(argv[1]) < 0)
    {
        fprintf(stderr, "%d doesn't match, <integer_value> must be >= 0\n", atoi(argv[1]));
        return -1;
    }

    int iterationTotal = atoi(argv[1]);
    iterationChild = iterationTotal / NUM_THREADS;
    int rc;

    pthread_t tid[NUM_THREADS];
    void *status;
    int inCirclePoints = 0;

    clock_t start, end;
    double executionTime;
    start = clock();

    for (int i = 0; i < NUM_THREADS; i++)
    {
        rc = pthread_create(&tid[i], NULL, runner, (void *)(intptr_t) i);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tid[i], &status);
        inCirclePoints += *(int *)status; //keep track of the total in count
    }
    double pi = 4 * (double)inCirclePoints / iterationTotal;

    printf("Multi-thread: With number of iterations = %d, estimate of pi is %g \n", iterationTotal, pi);

    end = clock();
    executionTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution Time: %.5f seconds\n", executionTime);

    pthread_exit(NULL);
    return 0;
}
