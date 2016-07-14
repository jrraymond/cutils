#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "dense_graph.h"


DENSE_GRAPH_INIT(u8, uint8_t, char, char)

void print_u8(uint8_t i) {
  printf("%u", i);
}

void print_char(char c) {
  printf("%c", c); 
}


void test_graph() {
  struct DenseGraph_u8_t g;
  dg_u8_init(&g, 4, 4);

  /* first we add 4 nodes */
  uint8_t nodes[8] = {0};
  char node_data[8];

  printf("Adding 4 nodes:\n");
  for (uint8_t i=0; i<4; ++i) {
    char nd = (char) ((uint8_t) 'a' + i);
    uint8_t node_index = dg_u8_add_node(&g, nd);
    nodes[i] = node_index;
    node_data[nodes[i]] = nd;
    printf("\t%u: %u->%c\n", i, node_index, nd);
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
      char ed = 'a' + (char) 4*i + j;
      uint8_t edge_index = dg_u8_add_edge(&g, u, v, ed);
      edges[4*i+j] = edge_index;
      edge_data[edge_index] = ed;
      printf("\t%u: %u->%c\n", 4*i+j, edge_index, ed);
    }
  }
  assert(g.num_edges == 8);

  /* then we add more edges to force dynamic array of edge data to grow */
  printf("Adding 8 more edges\n");
  for (uint8_t i=0; i<4; ++i) {
    for (uint8_t j=2; j<4; ++j) {
      uint8_t u = nodes[i];
      uint8_t v = nodes[j];
      char ed = 'A' + (char) 4*i + j;
      uint8_t edge_index = dg_u8_add_edge(&g, u, v, ed);
      edges[4*i+j] = edge_index;
      edge_data[edge_index] = ed;
      printf("\t%u: %u->%c\n", 4*i+j, edge_index, ed);
    }
  }
  assert(g.num_edges == 16);

  /* then we add more nodes to force graph matrix to grow */
  printf("Adding 4 more nodes:\n");
  for (uint8_t i=4; i<8; ++i) {
    char nd = (char) ((uint8_t) 'a' + i);
    uint8_t node_index = dg_u8_add_node(&g, nd);
    node_data[node_index] = nd;
    nodes[i] = node_index;
    printf("\t%u: %u->%c\n", i, node_index, nd);
  }
  printf("\n");
  assert(g.num_nodes == 8);

  _dg_u8_print(&g, &print_char, &print_char, &print_u8);

  /* then we test get node */
  printf("testing get_node\n");
  for (uint8_t i=0; i<8; ++i) {
    char nd;
    dg_u8_get_node(&g, nodes[i], &nd);
    printf("\t%u: %u->%c|%c\n", i, nodes[i], nd, node_data[nodes[i]]);
    assert(nd == node_data[nodes[i]]);
  }

  /* then we test get edge */
  printf("testing get_edge\n");
  for (uint8_t i=0; i<16; ++i) {
    char ed;
    dg_u8_get_edge(&g, edges[i], &ed);
    printf("\t%u: %u->%c|%c\n", i, edges[i], ed, edge_data[edges[i]]);
    assert(ed == edge_data[edges[i]]);
  }
  printf("testing node forward iteration\n");
  for (size_t itr = dg_u8_nodes_begin(&g);
      itr != dg_u8_nodes_end(&g);
      dg_u8_nodes_next(&g, &itr)
      ) {
    char nd;
    dg_u8_get_node(&g, itr, &nd);
    printf("\t%zu:%c\n", itr, nd);
  }

  printf("testing node backward iteration\n");
  size_t itr = dg_u8_nodes_end(&g);
  do {
    dg_u8_nodes_prev(&g, &itr);
    char nd;
    dg_u8_get_node(&g, itr, &nd);
    printf("\t%zu:%c\n", itr, nd);
  } while (itr != dg_u8_nodes_begin(&g));


  printf("testing del_node\n");
  /* test del node */
  for (uint8_t i=0; i<8; i += 2) {
    printf("%u,", nodes[i]);
    dg_u8_del_node(&g, nodes[i]);
  }
  printf("\n");

  printf("testing del_edge\n");
  /* test del edge */
  for (uint8_t i=0; i<16; i += 2) {
    printf("%u,", edges[i]);
    dg_u8_del_edge(&g, edges[i]);
  }
  printf("\n");

  printf("testing node forward iteration\n");
  for (size_t itr = dg_u8_nodes_begin(&g);
      itr != dg_u8_nodes_end(&g);
      dg_u8_nodes_next(&g, &itr)
      ) {
    char nd;
    dg_u8_get_node(&g, itr, &nd);
    printf("\t%zu:%c\n", itr, nd);
  }

  printf("testing node backward iteration\n");
  itr = dg_u8_nodes_end(&g);
  do {
    dg_u8_nodes_prev(&g, &itr);
    char nd;
    dg_u8_get_node(&g, itr, &nd);
    printf("\t%zu:%c\n", itr, nd);
  } while (itr != dg_u8_nodes_begin(&g));


  printf("DONE\n");
}

int main(int argc, char **argv) {

  test_graph();

  return EXIT_SUCCESS;
}
