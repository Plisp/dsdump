# dsdump

Reference implementations of various data structures optimized for clarity,
from which all else follows. Will be rewritten in a better language..eventually.

These implementations are **not intended as drop-in solutions** and so
particular edge cases are not handled for simplicity.
These are:
- handling failed allocations
- key/value memory management

However no assumptions about these values are made; sentinel tricks assuming
particular values of keys are avoided and left as an exercise to the reader.

- [x] ring deque (C)
- [x] binary heap (C)
- [x] skiplist (todo rewrite C++)
- [x] disjoint set union (C++)
- [ ] AVL tree (C)
- [ ] fenwick tree
- [ ] treap
- [ ] ab-trees
  - [ ] B-tree
  - [ ] B+tree
  - [ ] CSB+tree
- [ ] radix trees
  - [x] radix (patricia) trie (todo rewrite C++, work with std::strings)
  - [ ] level compresssed (linux LPC-trie)
  - [ ] adaptive spans (ART)
- [ ] hash tables
  - [ ] linear/quadratic probing
  - [ ] robin hood
  - [ ] cuckoo
- [ ] bloom filter
