# dsdump

Reference implementations of various data structures optimized for clarity,
from which all else follows. Will be tested, but not generic.

These implementations are **not intended as drop-in solutions** and so
particular edge cases are not handled for simplicity.
These are:
- handling failed allocations
- key/value memory management

All keys are integers and values are C-strings.
However no assumptions about these values are made; sentinel tricks assuming
particular values of keys are avoided and left as an exercise to the reader.

- [x] ring deque
- [x] binary heap
- [x] skiplist
- [ ] red-black tree
- [ ] treap
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
- [ ] bloom filter
