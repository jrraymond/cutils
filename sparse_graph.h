#ifndef __SPARSE_GRAPH_H
#define __SPARSE_GRAPH_H

/* TODO: linked list implementation */
#define SPARSE_GRAPH_TYPE(index_t) \
  struct SparseGraph { \
    size_t num_nodes; \
    size_t num_edges; \
    index_t *nodes; \
  }; \


#endif
