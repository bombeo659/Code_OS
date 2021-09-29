//factorial.c
#include "factorial.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *factorial(const int aNumber)
{
    char *arrayChar = NULL;
    arrayChar = malloc(sizeof(char) * aNumber);
    unsigned long long int fac = 1;
    for (int i = 1; i <= aNumber; i++)
    {
        fac *= i;
    }
    sprintf(arrayChar, "%llu", fac);
    return arrayChar;
}
