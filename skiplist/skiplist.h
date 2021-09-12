#include <stdio.h>
#include <stdlib.h>

typedef struct skiplist Skiplist;

Skiplist *skiplist_new();

char *skiplist_search(Skiplist *sk, long key);

Skiplist *skiplist_insert(Skiplist *sk, long key, char *val);
char *skiplist_delete(Skiplist *sk, long key);

void skiplist_foreach(Skiplist *sk, void (*cb)(Skiplist *));
