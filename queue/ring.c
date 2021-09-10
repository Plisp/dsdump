#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a)>(b)?(a):(b))

// implements a ring queue (deque) supporting pushing/popping from both ends
// we pop from elems[tail] and push to elems[head] (which is outside the queue)
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

// note this works until q->head rolls over twice, which cannot happen with
// an array fitting in size_t, so we're fine here and elsewhere
size_t queue_count(Queue *q) { return q->head - q->tail; };

static void resize(Queue *q) {
	size_t count = q->head - q->tail;
	long *newbuf = malloc(MAX(1, 2*count) * sizeof(long));
	// TODO check if the division is impacting performance. branch instead?
	for (size_t i = 0; i < count; i++)
		newbuf[i] = q->elems[(q->tail + i) % q->cap];
	q->cap = 2*count;
	q->tail = 0;
	q->head = count;
	free(q->elems);
	q->elems = newbuf;
}

// pushes item onto the back of Q in O(1) amortized
void queue_push(Queue *q, long item)
{
	if (q->head - q->tail == q->cap) resize(q);
	q->elems[q->head % q->cap] = item;
	q->head++;
}

// removes and returns the first element in the queue in O(1) amortized
// returns -1 if the queue is empty
long queue_pop(Queue *q)
{
	if (q->tail == q->head)
		return -1;
	long item = q->elems[q->tail % q->cap];
	q->tail++;
	// Guarantees amortized O(1), as we must've popped at least cap/2-cap/3
	// = cap/6 elements before we now move cap/3 elements.
	//
	// Resizing at cap/2 would be catastrophic O(n) for fluctuating push/pops
	// when the queue is around halfway full
	if (q->head - q->tail <= q->cap/3)
		resize(q);
	return item;
}

// pushes item onto the front of Q in O(1) amortized
void queue_push_front(Queue *q, long item)
{
	if (q->head - q->tail == q->cap) resize(q);
	q->tail--;
	q->elems[q->tail % q->cap] = item;
}

// removes and returns the last element in the queue in O(1) amortized
// returns -1 if the queue is empty
long queue_pop_back(Queue *q)
{
	if (q->tail == q->head)
		return -1;
	q->head--;
	long item = q->elems[q->head % q->cap];
	if (q->head - q->tail <= q->cap/3)
		resize(q);
	return item;
}

#ifdef TEST
int main() {
	Queue *q = queue_new();
	queue_free(q);
}
#endif
