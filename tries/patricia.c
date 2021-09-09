//
// patricia (crit-bit) trie heavily inspired by Tony Finch's elegant impl,
// available at https://github.com/fanf2/qp
//

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// string keys
struct inner {
	union Trie *child;
	long tag : 1,  // little-endian tag
		 index : 63; // bit-index growing from low to high bits
};

struct leaf {
	long key;
	// force alignment so that low bits are clear for distinguishing from an
	// inner node. Whatever value type used here must be distinguishable from
	// an inner node index field
	void *val;
};

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
void find_child(Trie **node, long key) {
	// for a string key you would instead extract key[i/8] and shift right
	// return (i/8 < len) ? (key[i/8] >> i%8 & 1) : 0;
	struct inner *inner = (struct inner *) *node;
	int dir = key >> inner->index & 1;
	*node = &inner->child[dir];
}

// returns the value associated with KEY
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
	// TODO verify empty case
	Trie *node = root;
	while (node->inner.tag)
		find_child(&node, key);
	// for strings...
	// size_t i;
	// for(i = 0; i <= len; i++) {
	//   if(key[i] != t->leaf.key[i])
	//     goto newkey;
	// }
	// // handle matching key...
	// newkey:
	// uint k1 = (byte)key[i],  k2 = (byte)t->leaf.key[i];
	// uint b = (uint)__builtin_clz((k1 ^ k2) << 24 | 0x800000);
	// index = 8 * i + (7 - b);
	long oldkey = node->leaf.key;
	if (key == oldkey) {
		node->leaf.val = val;
		return;
	}
	int i = 64 - __builtin_clzl(key ^ oldkey);
	int isset = key >> i & 1;
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

// deletes and returns the value associated with KEY in O(k)
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

int main() {
	;
}
