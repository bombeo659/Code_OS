// main.c

#include "readline.h"
#include "factorial.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    // Implement myfactorial
    char input[BUFSIZ] = "";

    while(1){
	scanf("%s", input);
    	if (read_line(input)) {
        	int aNumber;
        	sscanf(input, "%d", &aNumber);
        	printf("%s\n", factorial(aNumber));
    	}
	else 
		printf("%d\n", -1);
    }
    return 0;
}
