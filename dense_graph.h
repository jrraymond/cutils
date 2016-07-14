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
 *
 * the graph will grow but will never shrink
 */
#define DG_GROWTH_FACTOR 2

#define DENSE_GRAPH_TYPE(name, index_t, node_data_t, edge_data_t) \
  \
  MATRIX_TYPE(index_t, index_t) \
  \
  struct _EdgeData_##name##_t { \
    index_t u; \
    index_t v; \
    edge_data_t data; \
  }; \
  \
  struct DenseGraph_##name##_t { \
    index_t num_nodes; \
    index_t num_edges; \
    index_t next_node_index; \
    index_t next_edge_index; \
    struct Matrix_##index_t##_t matrix; \
    struct DynArray node_data; \
    struct DynArray edge_data; \
    struct DynArray node_indices; \
    struct DynArray edge_indices; \
  }; \
  \
  void dg_##name##_init(struct DenseGraph_##name##_t *g, index_t node_capacity, index_t edge_capacity) ; \
  \
  index_t dg_##name##_add_node(struct DenseGraph_##name##_t *g, node_data_t nd) ; \
  \
  index_t dg_##name##_add_edge(struct DenseGraph_##name##_t *g, index_t u, index_t v, edge_data_t ed) ; \
  \
  void dg_##name##_del_node(struct DenseGraph_##name##_t *g, index_t node) ; \
  \
  void dg_##name##_del_edge(struct DenseGraph_##name##_t *g, index_t edge) ; \
  \
  void dg_##name##_get_node(struct DenseGraph_##name##_t *g, index_t node, node_data_t *nd) ; \
  \
  void dg_##name##_get_node_ref(struct DenseGraph_##name##_t *g, index_t node, node_data_t **nd) ; \
  \
  void dg_##name##_get_edge(struct DenseGraph_##name##_t *g, index_t edge, edge_data_t *ed) ; \
  \
  void dg_##name##_get_edge_ref(struct DenseGraph_##name##_t *g, index_t edge, edge_data_t **ed) ; \
  \
  void _dg_##name##_print( \
      struct DenseGraph_##name##_t *g, \
      void (*print_node_data)(node_data_t nd), \
      void (*print_edge_data)(edge_data_t ed), \
      void (*print_index_t)(index_t i)) ; \


#define DENSE_GRAPH_IMPL(name, index_t, node_data_t, edge_data_t) \
  \
  MATRIX_IMPL(index_t, index_t) \
  \
  void dg_##name##_init(struct DenseGraph_##name##_t *g, index_t node_capacity, index_t edge_capacity) { \
    g->num_nodes = 0; \
    g->num_edges = 0; \
    matrix_##index_t##_init(&g->matrix, node_capacity, node_capacity); \
    da_DynArray_init(&g->node_data, node_capacity, sizeof(node_data_t)); \
    da_DynArray_init(&g->edge_data, edge_capacity, sizeof(struct _EdgeData_##name##_t)); \
    g->next_node_index = 0; \
    da_DynArray_init(&g->node_indices, 0, sizeof(index_t)); \
    g->next_edge_index = 0; \
    da_DynArray_init(&g->edge_indices, 0, sizeof(index_t)); \
  } \
  \
  index_t dg_##name##_add_node(struct DenseGraph_##name##_t *g, node_data_t nd) { \
    if (g->num_nodes >= g->matrix.rows) { \
      index_t nxt = g->num_nodes * DG_GROWTH_FACTOR; \
      matrix_##index_t##_resize(&g->matrix, nxt, nxt); \
    } \
    /* get the next available index */ \
    index_t next_index; \
    if (g->node_indices.size > 0) { \
      da_get(&g->node_indices, g->node_indices.size - 1, &next_index); \
      da_pop(&g->node_indices); \
    } else { \
      next_index = g->next_node_index; \
      ++g->next_node_index; \
    } \
    \
    /* set the node data and return the index */ \
    if (next_index >= g->node_data.size) { \
      da_append(&g->node_data, (void*) &nd); \
    } else { \
      da_set(&g->node_data, next_index, (void*) &nd); \
    } \
    ++g->num_nodes; \
    return next_index; \
  } \
  \
  index_t dg_##name##_add_edge(struct DenseGraph_##name##_t *g, index_t u, index_t v, edge_data_t ed) { \
    index_t next_index; \
    if (g->edge_indices.size > 0) { \
      da_get(&g->edge_indices, g->edge_indices.size - 1, &next_index); \
      da_pop(&g->edge_indices); \
    } else { \
      next_index = g->next_edge_index; \
      ++g->next_edge_index; \
    } \
    \
    if (next_index >= g->edge_data.size) { \
      da_append(&g->edge_data, (void*) &ed); \
    } else { \
      da_set(&g->edge_data, next_index, (void*) &ed); \
    } \
    ++g->num_edges; \
    return next_index; \
  } \
  \
  void dg_##name##_del_node(struct DenseGraph_##name##_t *g, index_t node) { \
    /*make the index available again */ \
    if (node != g->next_node_index - 1) { \
      da_append(&g->node_indices, (void*) &node); \
    } else { \
      --g->next_node_index; \
    } \
    /*clear all edge data */ \
    for (index_t i=0; i<g->matrix.rows; ++i) { \
      matrix_##index_t##_set(&g->matrix, i, node, -1); \
      matrix_##index_t##_set(&g->matrix, node, i, -1); \
    } \
    --g->num_nodes; \
  } \
  \
  void dg_##name##_del_edge(struct DenseGraph_##name##_t *g, index_t edge) { \
    if (edge != g->next_edge_index - 1) { \
      da_append(&g->edge_indices, (void*) &edge); \
    } else { \
      --g->next_edge_index; \
    } \
    struct _EdgeData_##name##_t *ed_p; \
    da_get(&g->edge_data, edge, (void**) &ed_p); \
    matrix_##index_t##_set(&g->matrix, ed_p->u, ed_p->v, -1); \
    da_append(&g->edge_indices, (void*) &edge); \
    --g->num_edges; \
  } \
  \
  void dg_##name##_get_node(struct DenseGraph_##name##_t *g, index_t node, node_data_t *nd) { \
    da_get(&g->node_data, node, (void*) nd); \
  } \
  \
  void dg_##name##_get_node_ref(struct DenseGraph_##name##_t *g, index_t node, node_data_t **nd) { \
    da_get_ref(&g->node_data, node, (void**) nd); \
  } \
  \
  void dg_##name##_get_edge(struct DenseGraph_##name##_t *g, index_t edge, edge_data_t *ed) { \
    da_get(&g->edge_data, edge, (void*) ed); \
  } \
  \
  void dg_##name##_get_edge_ref(struct DenseGraph_##name##_t *g, index_t edge, edge_data_t **ed) { \
    da_get_ref(&g->edge_data, edge, (void**) ed); \
  } \
  \
  void _dg_##name##_print(struct DenseGraph_##name##_t *g, void (*print_node_data)(node_data_t nd), void (*print_edge_data)(edge_data_t ed), void (*print_index_t)(index_t i)) { \
    printf("Node data:\n"); \
    for (size_t i=0; i<g->node_data.size; ++i) { \
      printf("%zu->", i); \
      node_data_t nd; \
      da_get(&g->node_data, i, (void*) &nd); \
      print_node_data(nd); \
      printf(","); \
    } \
    printf("\n"); \
  } \


#define DENSE_GRAPH_INIT(name, index_t, node_data_t, edge_data_t) \
  DENSE_GRAPH_TYPE(name, index_t, node_data_t, edge_data_t) \
  DENSE_GRAPH_IMPL(name, index_t, node_data_t, edge_data_t) \


#endif
