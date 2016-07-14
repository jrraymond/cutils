#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "dense_graph.h"


DENSE_GRAPH_INIT(u8, uint8_t, char, char)


void test_graph() {
  struct DenseGraph_u8_t g;
  dg_u8_init(&g, 4, 4);

  /* first we add 4 nodes */
  uint8_t nodes[8] = {0};
  char node_data[8];

  printf("Adding 4 nodes\n");
  for (uint8_t i=0; i<4; ++i) {
    char nd = (char) ((uint8_t) 'a' + i);
    node_data[i] = nd;
    nodes[i] = dg_u8_add_node(&g, nd);
  }
  assert(g.num_nodes == 4);

  /* then we add 8 edges */
  uint8_t edges[16] = {0};
  char edge_data[16];
  printf("Adding 8 edges\n");
  for (uint8_t i=0; i<4; ++i) {
    for (uint8_t j=0; j<2; ++j) {
      uint8_t u = nodes[i];
      uint8_t v = nodes[j];
      char ed = (char) u*v;
      edges[4*i+j] = dg_u8_add_edge(&g, u, v, ed);
      edge_data[4*i+j] = ed;
    }
  }
  assert(g.num_edges == 8);

  /* then we add more edges to force dynamic array of edge data to grow */
  printf("Adding 8 more edges\n");
  for (uint8_t i=0; i<4; ++i) {
    for (uint8_t j=2; j<4; ++j) {
      uint8_t u = nodes[i];
      uint8_t v = nodes[j];
      edges[i+j] = dg_u8_add_edge(&g, u, v, (char) u*v);
    }
  }
  assert(g.num_edges == 16);

  /* then we add more nodes to force graph matrix to grow */
  printf("Adding 4 more nodes\n");
  for (uint8_t i=4; i<8; ++i) {
    char nd = (char) ((uint8_t) 'a' + i);
    node_data[i] = nd;
    nodes[i] = dg_u8_add_node(&g, nd);
  }
  assert(g.num_nodes == 8);

  /* then we test get node */
  printf("testing get_node\n");
  for (uint8_t i=0; i<8; ++i) {
    char nd;
    dg_u8_get_node(&g, nodes[i], &nd);
    assert(nd == node_data[i]);
  }

  /* then we test get edge */
  printf("testing get_edge\n");
  for (uint8_t i=0; i<16; ++i) {
    char ed;
    dg_u8_get_edge(&g, edges[i], &ed);
    assert(ed == edge_data[i]);
  }

  /* test del node */

  /* test del edge */
}

int main(int argc, char **argv) {

  test_graph();

  return EXIT_SUCCESS;
}
