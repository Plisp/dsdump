#include <stdio.h>
#include <stdlib.h>

// implements a max-heap that grows by a factor of 2 when full
// children of x exist at 2*x and 2*x+1, parent is at x/2 truncated
//       [1]
//    [2]   [3]
// [4] [5] [6] [7]
//
// note that the initial size is 1 (counting the sentinel), so that with
// capacity 2, we never try to shrink and expanding works simply by x2
typedef struct heap {
	size_t size, cap;
	long *data; //[sentinel][heap][ ][ ]
} Heap;

Heap *heap_new() {
	struct heap *new = malloc(sizeof *new);
	new->size = 1;
	new->cap = 2;
	return new;
}

void heap_free(Heap *heap) { free(heap->data); free(heap); }

// returns the number of items currently in the heap in O(1)
size_t heap_count(Heap *heap) { return heap->size; }

// returns the top item in the heap in O(1) time
long heap_peek(Heap *heap) { return heap->data[1]; }

void swap(long *a, long *b) { long tmp = *a; *a = *b; *b = tmp; }

// adds an item to the heap in O(log n) time
void heap_push(Heap *heap, long item) {
	if (heap->size == heap->cap) {
		heap->data = realloc(heap->data, heap->cap*2);
		heap->cap *= 2;
	}
	heap->data[heap->size] = item;
	long *a = heap->data;
	size_t i = heap->size, parent = i/2;
	//
	while (parent != 0 && a[i] > a[parent]) {
		swap(&a[i], &a[parent]);
		i = parent, parent = i/2;
	}
	heap->size++;
}

// returns and removes the largest item from HEAP in O(log n) time
long heap_pop(Heap *heap) {
	if (heap->size == 1) return -1;
	if (heap->size < heap->cap/2) {
		heap->data = realloc(heap->data, heap->cap/2);
		heap->cap /= 2;
	}
	long popped = heap_peek(heap);
	heap->data[1] = heap->data[heap->size-1];
	// sift data[1] down
	long *a = heap->data;
	for (size_t i, max_child = 1, left, right;;) {
		i = max_child;
		left = i*2, right = i*2 + 1;
		if (right >= heap->size) break;
		// swap with larger child > other child
		max_child = left + (a[left] < a[right]);
		if (a[max_child] > a[i])
			swap(&a[max_child], &a[i]);
		else break;
	}
	heap->size--;
	return popped;
}

int main() {
	Heap *heap = heap_new();
	heap_free(heap);
}
