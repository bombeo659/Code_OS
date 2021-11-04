#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t *q)
{
	return (q->size == 0);
}

void enqueue(struct queue_t *q, struct pcb_t *proc)
{
	/* TODO: put a new process to queue [q] */
	if (q->size == MAX_QUEUE_SIZE)
		return;
	q->proc[q->size++] = proc;
}

struct pcb_t *dequeue(struct queue_t *q)
{
	/* TODO: return a pcb whose prioprity is the highest
	 * in the queue [q] and remember to remove it from q
	 * */
	if (empty(q))
		return NULL;
	int highestPrioprity = 0;
	int i, index = 0;
	// find index at pcb whose prioprity is the highest in the queue
	for (i = 0; i < q->size; i++)
	{
		if (q->proc[i]->priority > highestPrioprity)
		{
			highestPrioprity = q->proc[i]->priority;
			index = i;
		}
	}
	// remove and return pcb found
	struct pcb_t *pcbReturn = q->proc[index];
	q->proc[index] = q->proc[--q->size];
	return pcbReturn;
}
