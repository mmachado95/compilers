#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  char *name;
  struct node **children;
  struct node *brother;
} node_t;

node_t *ast;

node_t* insert_to_ast(char *node_name, int n_args, ... );
