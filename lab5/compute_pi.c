#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define SEED 35791246

double comute_pi(int niter)
{
    /* initialize random numbers */
    double x, y;
    int i = 0, count = 0; /* # of points in the 1st quadrant of unit circle */
    double z;
    double pi = 0;
    srand(SEED);
    for (i = 0; i < niter; i++)
    {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        z = x * x + y * y;
        if (z <= 1)
            count++;
    }
    return pi = (double)count / niter * 4;
}

int main(int argc, char *argv)
{
    int niter = 0;

    double pi;

    printf("Enter the number of iterations used to estimate pi: ");
    scanf("%d", &niter);

    pi = comute_pi(niter);

    printf("# of trials= %d , estimate of pi is %g \n", niter, pi);
}