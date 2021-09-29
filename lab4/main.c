#include <stdio.h>
#include <stdlib.h>
#include "ex1.h"
int main(){
	unsigned int size, align;
	while(1){
		printf("Enter the size (unsigned int) = ");
		scanf("%u", &size);
		printf("Enter the align (unsigned int) = ");
		scanf("%u", &align);

       	unsigned int *p = (unsigned int *)aligned_malloc(size, align);
        if(p != NULL){
       	    printf ("Address: %p\n", p);
		    aligned_free (p);
        }
        else 
            printf("Result is NULL, pls input again!!!\n");
	}
	return 0;
}