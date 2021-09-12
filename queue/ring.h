#include <stdio.h>
#include <stdlib.h>

typedef struct queue Queue;

Queue *queue_new();
void queue_free(Queue *q);

size_t queue_count(Queue *q);

void queue_push(Queue *q, long item);
long queue_pop(Queue *q);
void queue_push_front(Queue *q, long item);
long queue_pop_back(Queue *q);
