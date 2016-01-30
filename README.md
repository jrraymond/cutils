cutils

c library with frequently needed data-structures and algorithms.

todo:
benchmarks
more unit tests for vector and hashtable
hashtable get rid of bitmask to check if slot is occupied, insead use constant NULL, DUMMY
so the possible states are:
  unused: key==NULL
  dummy: key==1
  active: key>1
Rehashing the hastable, both for size increases and decreases

Other data structures:
map
set
linked list
