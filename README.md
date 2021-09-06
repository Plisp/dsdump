# dsdump

Reference implementations of various data structures optimized for clarity,
from which all else follows. Will be tested, but not generic.

These implementations are **not intended as drop-in solutions** and so
various edge cases are not handled for simplicity.
These include handling failed memory allocation, general key/value ownership
semantics and error return values.

All keys are nonnegative integers and values are C-strings.
However no assumptions about these values are made; sentinel tricks assuming
particular values of keys are avoided and left as an exercise to the reader.

- [x] ring deque
- [x] binary heap
- [x] skiplist
- [ ] red-black tree
- [ ] ab-trees
  - [ ] B-tree
  - [ ] B+tree
  - [ ] CSB+tree
- [ ] radix trees
  - [ ] level compresssed (LPC-trie)
  - [ ] adaptive spans (ART)
  - [ ] my own thing...
- [ ] hash tables
  - [ ] linear/quadratic probing
  - [ ] robin hood
  - [ ] cuckoo
- [ ] treap
- [ ] bloom filter
