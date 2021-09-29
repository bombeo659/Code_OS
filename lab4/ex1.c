#include<stdio.h>
#include<stdlib.h>
#include"ex1.h"
#include<math.h>
#include<stdbool.h>
#include <stdint.h>

bool isPowerOfTwo(unsigned int n)
{
   if(n<=0) return false;
   return (ceil(log2(n)) == floor(log2(n)));
}

void *aligned_malloc(unsigned int size, unsigned int align)
{
    if(!isPowerOfTwo(align)) return NULL;
    if(size == 0) return NULL;

    void *ptr;
    void *p = malloc(size + align - 1 + sizeof(void*));

    if(p != NULL){
    	ptr = (void*)(((uintptr_t)p + sizeof(void*) + (uintptr_t)align-1) & ~((uintptr_t)align-1));

    	*((void**)((uintptr_t)ptr - sizeof(void*))) = p;

     	return ptr;
    }
    return NULL;
}

void* aligned_free(void *ptr)
{
    void* p = *((void**)((uintptr_t)ptr - sizeof(void*)));
    free(p);
}
