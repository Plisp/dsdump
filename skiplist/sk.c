#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_HEIGHT 16

// maintains a map of int keys to strings, in increasing order
// you must inline the skiplist head (it saves an indirection)
typedef struct skiplist {
	long key;
	char *val; // valid null-terminated strings for now
	struct skiplist *tower[MAX_HEIGHT+1]; // a null sentinel lies atop the tower
} Skiplist;

Skiplist *skiplist_new() { // free by deleting all entries and free()ing the head
	Skiplist *new = malloc(sizeof *new);
	*new = (struct skiplist) {0};
	return new;
}

// Generates a random valid height between 1 and MAX_HEIGHT
// TODO using a flexible array member for towers is probably a good idea if
// the data set is large. this needs to be modified to generate larger heights
int rand_height() {
	int cap = 1 << MAX_HEIGHT-1; // this guarantees we hit a maximum, not UB
	return __builtin_ctz(rand() | cap) + 1;
}

// returns the string value corresponding to KEY in SK in O(log n) average time
char *skiplist_search(Skiplist *sk, long key)
{
	int height = 0;
	while (sk->tower[height]) {
		height++;
	}
	while (--height >= 0) {
		while (sk->tower[height] && key >= sk->tower[height]->key)
			sk = sk->tower[height];
	}
	return (sk->key == key) ? sk->val : NULL;
}

// inserts VAL into SK with key KEY in O(log n) average time
Skiplist *skiplist_insert(Skiplist *sk, long key, char *val)
{
	Skiplist *head = sk;
	int height = MAX_HEIGHT;
	int new_height = rand_height(); // count of nodes, not index
	Skiplist **stack[new_height+1]; // constant bounded VLA
	stack[new_height] = NULL; // a sentinel so there's only one condition VVV

	while (--height >= 0) {
		while (sk->tower[height] && key > sk->tower[height]->key) {
			sk = sk->tower[height];
		}
		if (height+1 <= new_height) // height is an index
			stack[height] = &sk->tower[height];
	}
	Skiplist *new = skiplist_new();
	new->key = key;
	new->val = val;
	while (stack[++height]) {
		Skiplist *old = *stack[height];
		*stack[height] = new;
		new->tower[height] = old;
	}
	return head;
}

// returns the value corresponding to KEY, and deletes it from the skiplist,
// (will be NULL if KEY does not exist) in O(log n) average time
char *skiplist_delete(Skiplist *sk, long key)
{
	if (!sk->tower[0]) return NULL;

	int height = MAX_HEIGHT;
	while (--height >= 0) {
		while (sk->tower[height] && key > sk->tower[height]->key) {
			sk = sk->tower[height];
		}
		if (sk->tower[height] && sk->tower[height]->key == key) {
			Skiplist *found = sk->tower[height];
			sk->tower[height] = found->tower[height];
			if (height == 0) {
				char *val = found->val;
				free(found);
				return val;
			}
		}
	}
	return NULL;
}

void skiplist_print_node(Skiplist *sk) {
	printf("%lx k: %3ld v: %6s ", (long)sk>>4&0xff, sk->key, sk->val);
	int height = 0;
	do {
		putchar('[');
		putchar(sk->tower[height] ? '*' : ' ');
		printf("%lx", (long)sk->tower[height]>>4&0xff);
		putchar(']');
	} while (sk->tower[height++]);
	puts("");
}

// iterates through all entries in the skiplist. O(n)
void skiplist_foreach(Skiplist *sk, void (*cb)(Skiplist *)) {
	while (sk = sk->tower[0]) cb(sk);
}

int main() {
	srand(time(NULL));
	Skiplist *sk = skiplist_new();
	for (int i = 0; i < 1000000; i++) {
		skiplist_insert(sk, rand(), "test");
	}
	/*
	skiplist_insert(sk, 1, "hello");
	skiplist_foreach(sk, &skiplist_print_node);
	skiplist_insert(sk, 63, "test");
	skiplist_insert(sk, 22, "test");
	skiplist_insert(sk, 64, "test");
	skiplist_insert(sk, 65, "test");
	skiplist_insert(sk, 66, "test");
	skiplist_insert(sk, 23, "test");
	skiplist_insert(sk, 24, "test");
	skiplist_insert(sk, 25, "delme");
	printf("%s matches 25!\n", skiplist_search(sk, 25));
	skiplist_foreach(sk, &skiplist_print_node);
	char *val = skiplist_delete(sk, 25);
	printf("deleting 25 - %s!\n", val);
	skiplist_foreach(sk, &skiplist_print_node);
	*/
	return 0;
}
