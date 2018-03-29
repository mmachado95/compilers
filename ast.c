#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "ast.h"

node_t* create_node(char *type, char *value) {
  // allocate memory for new node
  node_t *n = (node_t*) malloc(sizeof (node_t));

  // save strings (save lines by using strdup)
  n->type = strdup(type);
  if (value == NULL) {
    n->value = value;
  } else {
    n->value = strdup(value);
  }

  n->child = NULL;
  n->sibling = NULL;
  return n;
}

// insert node in ast
node_t* insert_node(char *type, char *value, int n_args, ...) {
  int i;

  // list of extra args passed to function
  va_list args;
  // number of extra args
  va_start(args, n_args);

  if (value != NULL) {
    printf("\n\n\n\n");
    printf("%s\n", value );
    value = strdup(value);
    printf("%s\n", value );
    printf("\n\n\n\n");

  }

  // create node
  node_t *new_node = create_node(type, value);


  // iterate extra args
  for (i = 0; i < n_args; i++) {
    node_t *n = va_arg(args, node_t *);
    if (n != NULL) {
      if (new_node->child == NULL) {
        new_node->child = n;
      }
      else {
        printf("Adding child to node %s!! Sibling %s\n\n\n", new_node->type, new_node->child->type);
        printf("value: %s\n", n->type);
        add_sibling(new_node, n);
        new_node = n;
      }
    }
  }

  va_end(args);
  return new_node;
}

// adds sibling to node
node_t *add_sibling(node_t *original, node_t *sibling) {
  node_t *aux = original;

  while (aux->sibling != NULL) {
    aux = aux->sibling;
  }

  aux->sibling = sibling;
  return original;
}

void print_ast(node_t *n) {
  printf("%s\n", n->type);
  if (n->child != NULL) {
    print_ast(n->child);
  }
  if (n->sibling != NULL) {
    print_ast(n->sibling);
  }
}
