//
// patricia (crit-bit) trie heavily inspired by Tony Finch's elegant impl,
// available at https://github.com/fanf2/qp
//

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct inner {
	union Trie *child;
	long tag : 1,     // XXX this assumes 64-bit little endian
		 index : 63; // index of diffientiating bit (from high to low)
};

struct leaf {
	long key;
	// force alignment so that low bits are clear for distinguishing from an
	// inner node (which always has the lowest set). Whatever value type used
	// here must be distinguishable from the inner->index field
	void *val;
};

// a map of 64-bit integers to word-aligned value pointers
// For strings see https://github.com/fanf2/qp/blob/master/cb.c, though the
// idea should be clear using integers
typedef union Trie {
	struct inner inner;
	struct leaf leaf;
} Trie;

Trie *trie_new() {
	Trie *node = malloc(sizeof *node);
	*node = (Trie) {0};
	return node;
}

// sets *node to child[1] if KEY has a bit set at node->index, else child[0]
static void find_child(Trie **node, long key) {
	// for a string key you would instead extract key[i/8] and shift right
	// return (i/8 < len) ? (key[i/8] >> (7 - i%8) & 1) : 0;
	// note that i%8 is the distance from the more significant end of the key
	struct inner *inner = (struct inner *) *node;
	int dir = key >> (63 - inner->index) & 1;
	*node = &inner->child[dir];
}

// returns the value associated with KEY or NULL in O(k), k being key length
void *trie_search(Trie *root, long key)
{
	while (root->inner.tag)
		find_child(&root, key);
	return (key == root->leaf.key) ? root->leaf.val : NULL;
}

// inserts VAL under KEY in O(k)
void trie_insert(Trie *root, long key, void *val)
{
	assert(!((uintptr_t)val & 3));
	// in case of an empty trie, an empty 0 entry is created safety which may
	// be overwritten later
	Trie *node = root;
	while (node->inner.tag)
		find_child(&node, key);
	//
	long oldkey = node->leaf.key;
	if (key == oldkey) {
		node->leaf.val = val;
		return;
	}
	int i = __builtin_clzl(key ^ oldkey);
	int isset = key >> (63 - i) & 1;
	node = root;
	while (node->inner.tag) {
		if(i < node->inner.index)
			break;
		find_child(&node, key);
	}
	Trie *new_children = malloc(sizeof(Trie) * 2);
	Trie old = *node;
	node->inner.child = new_children;
	node->inner.tag = 1;
	node->inner.index = i;
	node->inner.child[isset] = (Trie) { .leaf = { .key = key, .val = val } };
	node->inner.child[!isset] = old;
}

// deletes and returns the value associated with KEY in O(k),
// or NULL if not found
void *trie_delete(Trie *trie, long key)
{
	Trie *root = trie, *parent = NULL;
	int dir;
	while (root->inner.tag) {
		parent = root;
		find_child(&root, key);
	}
	if(key != root->leaf.key)
		return NULL;
	void *val = root->leaf.val;
	// root deletion
	if(parent == NULL) {
		*trie = (Trie) {0};
		return val; // in an empty trie val will be NULL. root tag was also 0
	}
	Trie *children = parent->inner.child;
	*parent = parent->inner.child[!dir];
	free(children);
	return val;
}

#ifdef TEST
#include <string.h>
int main() {
	Trie *t = trie_new();
	char *s1 = aligned_alloc(8, 8); // word aligned 8-byte string
	strcpy(s1, "test");
	trie_insert(t, 1, s1);
	printf("1 maps to %s\n", (char *)trie_search(t, 1));
	//
	char *s2 = aligned_alloc(8, 8);
	strcpy(s2, "test2");
	trie_insert(t, 2, s2);
	printf("2 maps to %s\n", (char *)trie_search(t, 2));
	trie_delete(t, 1);
	printf("2 maps to %s\n", (char *)trie_search(t, 2));
}
#endif
