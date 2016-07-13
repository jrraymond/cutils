#ifndef __DENSE_GRAPH_H
#define __DENSE_GRAPH_H

#include "matrix.h"
#include "dynamic_array.h"

#include <stdint.h>

/* A dense graph is represented by an adjacency matrix
 *   a graph is dense if it has O(n^2) edges
 *
 * node_data holds the associated node data
 * edge_data holds the associated edge data
 * index_t is a bound on the number of indices the graph will need
 */

#define DENSE_GRAPH_TYPE(name, index_t, node_data_t, edge_data_t) \
  \
  MATRIX_TYPE(index_t, index_t) \
  \
  struct DenseGraph_##name##_t { \
    index_t num_nodes; \
    index_t num_edges; \
    Matrix_##index_t##_t matrix; \
    struct DynArray node_data; \
    struct DynArray edge_data; \
    struct DynArray indices; \
  }; \
  \
  void dg_##name##_init(DenseGraph_##name##_t *g, index_t node_capacity, index_t edge_capacity) ; \
  \
  index_t dg_##name##_add_node(DenseGraph_##name##_t *g, node_data_t nd) ; \
  \
  index_t dg_##name##_add_edge(DenseGraph_##name##_t *g, index_t u, index_t v, edge_data_t ed) ; \
  \
  bool dg_##name##_del_node(DenseGraph_##name##_t *g, index_t node) ; \
  \
  bool dg_##name##_del_edge(DenseGraph_##name##_t *g, index_t edge) ; \
  \


#define DENSE_GRAPH_IMPL(name, index_t, node_data_t, edge_data_t) \
  \
  MATRIX_IMPL(index_t, index_t) \
  \
  void dg_##name##_init(DenseGraph_##name##_t *g, index_t node_capacity, index_t edge_capacity) { \
    g->num_nodes = 0; \
    g->num_edges = 0; \
    Matrix_##index_t##_init(&g->matrix, node_capacity); \
    da_DynArray_init(&g->node_data, node_capacity, sizeof(node_data_t)); \
    da_DynArray_init(&g->edge_data, edge_capacity, sizeof(edge_data_t)); \
    da_DynArray_init(&g->indices, node_capacity, sizeof(index_t)); \
    for (index_t i=0; i<node_capacity; ++i) { \
      da_append(&g->indices, (void*) i); \
    } \
  } \
  \
  index_t dg_##name##_add_node(DenseGraph_##name##_t *g, node_data_t nd) { \
    if (g->num_nodes > mtrx_##index_t##_rows(&g->matrix)) { \
      /* TODO RESIZE */ \
    } \
    /* get the next available index */ \
    index_t next_index; \
    da_get(&g->indices, g->indices.size - 1, &next-index); \
    da_pop(&g->indices); \
    \


    
  } \
  \
  index_t dg_##name##_add_edge(DenseGraph_##name##_t *g, index_t u, index_t v, edge_data_t ed) { \
  } \
  \
  bool dg_##name##_del_node(DenseGraph_##name##_t *g, index_t node) { \
  } \
  \
  bool dg_##name##_del_edge(DenseGraph_##name##_t *g, index_t edge) { \
  } \
  \


#endif
