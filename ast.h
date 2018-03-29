#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  char *type;
  char *value;
  struct node *child;
  struct node *sibling;
} node_t;

node_t *ast;

node_t *create_node(char *type, char *value);
node_t *insert_node(char *type, char *value, int n_args, ...);
node_t *add_sibling(node_t *original, node_t *sibling);
void print_ast();
