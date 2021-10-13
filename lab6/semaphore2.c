#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_ITEMS 10
#define THREADS 2           // 1 producer and 1 consumer
#define LOOPS 2 * MAX_ITEMS // variable

// Ini t i a t e shared buf fer
int buffer[MAX_ITEMS];
int fill = 0;
int use = 0;

sem_t empty;
sem_t full;
sem_t look;

void put(int value); // put data into buffer
int get();           // get data from buffer
void *producer(void *arg)
{
    int i;
    int tid = (intptr_t)arg;
    for (i = 0; i < LOOPS; i++)
    {
        sem_wait(&look);
        sem_wait(&empty); // line P1
        put(i);           // line P2
        printf("Producer %d put data %d\n", tid, i);
        sleep(1);
        sem_post(&full); // line P3
        sem_post(&look);
    }
    pthread_exit(NULL);
    return NULL;
}
void *consumer(void *arg)
{
    int i, tmp = 0;
    int tid = (intptr_t)arg;
    while (tmp != -1)
    // for (i = 0; i < LOOPS; i++)
    {
        sem_wait(&look);
        sem_wait(&full); // line C1
        tmp = get();     // line C2
        printf("Consumer %d get data %d\n", tid, tmp);
        sleep(1);
        sem_post(&empty); // line C3
        sem_post(&look);
    }
    pthread_exit(NULL);
    return NULL;
}
int main(int argc, char **argv)
{
    int i, j;
    int tid[THREADS];
    pthread_t producers[THREADS];
    pthread_t consumers[THREADS];
    sem_init(&empty, 0, MAX_ITEMS);
    sem_init(&full, 0, 0);
    sem_init(&look, 0, 1);
    for (i = 0; i < THREADS; i++)
    {
        tid[i] = i;
        // Create producer thread
        pthread_create(&producers[i], NULL, producer, (void *)(intptr_t)tid[i]);
        // Create consumer thread
        pthread_create(&consumers[i], NULL, consumer, (void *)(intptr_t)tid[i]);
    }
    for (i = 0; i < THREADS; i++)
    {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }
    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_exit(NULL);
    return 0;
}
void put(int value)
{
    buffer[fill] = value;          // line f1
    fill = (fill + 1) % MAX_ITEMS; //  line f2
}
int get()
{
    int tmp = buffer[use];       // line g1
    use = (use + 1) % MAX_ITEMS; // line g2
    return tmp;
}