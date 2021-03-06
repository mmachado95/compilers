#ifndef AST_HEADER
#define AST_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "symbol_table.h"

typedef struct node {
  int has_error;
  int line, col;
  char *type;
  char *value;
  char *type_e; // type of element (int, ...)
  struct node *child;
  struct node *sibling;
} node_t;

node_t *ast;

node_t *create_node(int line, int col, char *type, char *value);
node_t *insert_node(int line, int col, char *type, char *value, int n_args, ...);
node_t *add_sibling(node_t *original, node_t *sibling);
node_t *make_node_correct(int line, int col, node_t *node_to_correct);
void insert_node_special(node_t *first, node_t *second);
void print_ast();
void destroy_ast(node_t *current);

#endif
