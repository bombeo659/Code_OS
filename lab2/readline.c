//file readline.c
#include "readline.h"

int read_line(char *str)
{
    while (*str != '\0')
    {
        if (*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}
