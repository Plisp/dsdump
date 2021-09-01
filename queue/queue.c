#include <stdio.h>
#include <stdlib.h>
#define MAX(a, b) ((a)>(b)?(a):(b))

typedef struct queue {
	size_t cap;
	size_t tail, head; // will unsigned wrap
	long *elems;
} Queue;

Queue *queue_new() {
	Queue *q = malloc(sizeof *q);
	*q = (Queue) {0};
	return q;
}

void queue_free(Queue *q) { // you better have popped all your allocations
	free(q->elems);
	free(q);
}

static void resize(Queue *q) {
	size_t count = q->head - q->tail;
	long *newbuf = malloc(MAX(1, 2*count) * sizeof(long));
	for (size_t i = 0; i < count; i++)
		newbuf[i] = q->elems[(q->tail + i) % q->cap];
	q->cap = 2*count;
	q->tail = 0;
	q->head = count;
	q->elems = newbuf;
}

// pushes item onto the back of Q in O(1) amortized
void queue_push(Queue *q, long item)
{
	if (q->head == q->tail + q->cap) resize(q);
	q->elems[q->head++ % q->cap] = item;
}

// removes and returns the first element in the queue in O(1) amortized
// returns -1 if the queue is empty
long queue_pop(Queue *q) {
	if (q->tail == q->head)
		return -1;
	long item = q->elems[q->tail++ % q->cap];
	// guarantees amortized O(1)
	if (q->head - q->tail <= q->cap/3)
		resize(q);
	return item;
}

int main() {
	Queue *q = queue_new();
	queue_free(q);
}
