#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_HEIGHT 7

// maintains a map of int keys to strings, in increasing order
// you must inline the skiplist head (it saves an indirection)
struct skiplist {
	long key;
	char *val; // valid null-terminated strings for now
	struct skiplist *tower[MAX_HEIGHT+1]; // a null sentinel lies atop the tower
};
typedef struct skiplist Skiplist;

Skiplist *skiplist_new() // free by deleting all entries and free()ing the head
{
	Skiplist *new = malloc(sizeof *new);
	*new = (struct skiplist) {0};
	return new;
}

int rand_height(int max) {
	int height = 1;
	while(rand() & 1)
		height++;
	return height > max ? max : height;
}

char *skiplist_search(Skiplist *sk, long key)
{
	int height = 0;
	while(sk->tower[height]) {
		height++;
	}
	while(--height >= 0) {
		while(sk->tower[height] && key >= sk->tower[height]->key)
			sk = sk->tower[height];
	}
	return (sk->key == key) ? sk->val : NULL;
}

// inserts VAL into SK with key KEY. Returns the new head of the skiplist
Skiplist *skiplist_insert(Skiplist *sk, long key, char *val)
{
	Skiplist *head = sk;
	// empty skiplist case
	if(!sk->val) {
		sk->key = key, sk->val = val;
		return head;
	}
	Skiplist *new = skiplist_new();
	new->key = key, new->val = val;
	// we don't really have the option of using a sentinel head with a generic
	// predicate, but you can change that and remove this case and fix
	// TODO repeated insertion before the head, which is currently catastrophic
	if(key < sk->key) {
		for(int i = 0; sk->tower[i]; i++)
			new->tower[i] = sk;
		return new;
	}
	int height = MAX_HEIGHT;
	int new_height = rand_height(MAX_HEIGHT); // count of nodes, not index
	Skiplist **stack[new_height+1]; // constant bounded VLA
	stack[new_height] = NULL; // a sentinel so there's only one condition VVV
	while(--height >= 0) {
		while(sk->tower[height] && key > sk->tower[height]->key)
			sk = sk->tower[height];
		if(height < new_height)
			stack[height] = &sk->tower[height];
	}
	while(stack[++height]) {
		Skiplist *old = *stack[height];
		*stack[height] = new;
		new->tower[height] = old;
	}
	return head;
}

// returns the new skiplist head after deleting the value corresponding
// to KEY, which is stored in *VAL (will be NULL if KEY does not exist)
Skiplist *skiplist_delete(Skiplist *sk, long key, char **val)
{
	Skiplist *head = sk;
	if(!sk->tower[0]) {
		sk->val = NULL;
		return head;
	}
	if(key == sk->key) { // head deletion case
		Skiplist *next = sk->tower[0]; // non null
		for(int i = MAX_HEIGHT; !next->tower[i]; i--)
			next->tower[i] = sk->tower[i];
		return next;
	}
	int height = MAX_HEIGHT;
	Skiplist **stack[MAX_HEIGHT+1]; // constant bounded VLA
	stack[MAX_HEIGHT] = NULL;
	//
	while(--height >= 0) {
		while(sk->tower[height] && key > sk->tower[height]->key)
			sk = sk->tower[height];
		stack[height] = &sk->tower[height];
	}
	sk = sk->tower[0]; // the one we want to delete, maybe
	if(sk->key == key) {
		while(sk->tower[++height])
			*stack[height] = sk->tower[height];
		*val = sk->val;
		free(sk);
	} else *val = NULL;
	return head; // fine if there's no deletion
}

void skiplist_print(Skiplist *sk, FILE *out) {
	if(!sk->val) {
		fprintf(out, "empty!\n");
		return;
	}
	do {
		fprintf(out, "%lx k: %3ld v: %6s ", (long)sk>>4&0xff, sk->key, sk->val);
		int height = 0;
		do {
			fputc('[', out);
			fputc(sk->tower[height] ? '*' : ' ', out);
			printf("%lx", (long)sk->tower[height]>>4&0xff);
			fputc(']', out);
		} while (sk->tower[height] && ++height < MAX_HEIGHT);
		sk = sk->tower[0];
		puts("");
	} while(sk);
}

int main() {
	srand(time(NULL));
	Skiplist *sk = skiplist_new();
	skiplist_insert(sk, 1, "hello");
	skiplist_print(sk, stdout);
	puts("\ntest\n");
#if 1
	struct skiplist second = {.key = 42, .val = "second"};
	struct skiplist first = {.key = 21, .val = "first", .tower = { &second }};
	skiplist_print(&first, stdout);
	printf("%s matches 42!\n", skiplist_search(&first, 42));
	skiplist_insert(&first, 63, "test");
	skiplist_insert(&first, 22, "test");
	skiplist_insert(&first, 64, "test");
	skiplist_insert(&first, 65, "test");
	skiplist_insert(&first, 66, "test");
	skiplist_insert(&first, 23, "test");
	skiplist_insert(&first, 24, "test");
	skiplist_insert(&first, 25, "delme");
	skiplist_print(&first, stdout);
	char *val;
	first = *skiplist_delete(&first, 25, &val);
	printf("deleting 25 - %s!\n", val);
	skiplist_print(&first, stdout);
#endif
	return 0;
}
