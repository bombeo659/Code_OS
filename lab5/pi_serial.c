#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define SEED 35791246

double compute_pi(int niter)
{
    double x, y, pi = 0;
    long long i, count = 0;

    srand(SEED);

    for (i = 0; i < niter; i++)
    {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        if ((x * x + y * y) <= 1) // nam trong vong trong ban kinh 1
            count++;
    }
    return pi = 4 * (double)count / niter;
}

int main(int argc, char *argv[])
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

    clock_t start, end;
    double executionTime;
    start = clock();

    double pi = compute_pi(iterationTotal);

    printf("Serial: With number of iterations = %d, estimate of pi is %g \n", iterationTotal, pi);

    end = clock();
    executionTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution Time: %.5f seconds\n", executionTime);

    return 0;
}
