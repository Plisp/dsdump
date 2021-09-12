#include <stdio.h>
#include <stdlib.h>

typedef struct heap Heap;

Heap *heap_new();
void heap_free(Heap *heap);

size_t heap_count(Heap *heap);

long heep_peek(Heap *heap);
void heap_push(Heap *heap, long item);
long heap_pop(Heap *heap);
