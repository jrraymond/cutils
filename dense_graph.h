#ifndef __DENSE_GRAPH_H
#define __DENSE_GRAPH_H

#include "matrix.h"
#include "dynamic_array.h"

#include <stdint.h>

/* A dense graph is represented by an adjacency matrix
 * indices holds indexes that are unused
 * node_data holds the associated node data
 * edge_data holds the associated edge data
 */

#define DENSE_GRAPH_TYPE(index_t, node_data_t, edge_data_t) \
  \
  MATRIX_TYPE(u32, uint32_t) \
  \
  struct DenseGraph { \
    size_t num_nodes; \
    size_t num_edges; \
    Matrix_u32_t matrix; \
    us_##node_data_t##_t node_data; \
    us_##edge_data_t##_t edge_data; \
    struct DynArray indices; \
  }; \


#endif
