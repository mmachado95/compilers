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

  n->children = NULL;
  n->brother = NULL;
  return n;
}

// insert node in ast
node_t* insert_node(char *type, char *value, int n_args, ...) {
  int i;

  // list of extra args passed to function
  va_list args;
  // number of extra args
  va_start(args, n_args);

  // create node
  node_t *new_node = create_node(type, value);

  node_t *aux;
  // iterate extra args
  for (i = 0; i < n_args; i++) {
    node_t *n = va_arg(args, node_t *);

    if (n != NULL) {
      if (new_node->children == NULL) {
        new_node->children = n;
        aux = n;
      }
      else {
        add_brother(aux, n);
        aux = n;
      }
    }
  }

  va_end(args);
  return new_node;
}

// adds brother to node
node_t *add_brother(node_t *original, node_t *brother) {
  node_t *aux = original;

  while (aux->brother != NULL) {
    aux = aux->brother;
  }

  aux->brother = brother;
  return original;
}
