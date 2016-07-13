#ifndef __DENSE_GRAPH_H
#define __DENSE_GRAPH_H

/* so much to think aboot */
#define DENSE_GRAPH_TYPE(index_t, node_data_t, edge_data_t) \
  struct DenseGraph { \
    size_t num_nodes; \
    size_t num_edges; \
    index_t **matrix; \
    us_##node_data_t##_t node_data; \
    us_##edge_data_t##_t edge_data; \
  }; \


#endif
