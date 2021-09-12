#include <stdio.h>
#include <stdlib.h>

typedef union trie Trie;

Trie *trie_new();

void *trie_search(Trie *root, long key);

void trie_insert(Trie *root, long key, void *val);
void *trie_delete(Trie *trie, long key);
