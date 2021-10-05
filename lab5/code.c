#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

void *hello(void *tid)
{
    printf("Hello from thread %ld\n", (intptr_t)tid);
    return NULL;
}

int main()
{
    pthread_t tid[10];
    int i;
    for (i = 0; i < 10; i++)
    {
        pthread_create(&tid[i], NULL, hello, (void *)(intptr_t)(i));
        pthread_join(tid[i], NULL); //wait thread child
    }

    pthread_exit(NULL);
}
