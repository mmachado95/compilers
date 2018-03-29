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

  return n;
}

// insert node in ast
node_t* insert_node(char *type, char *value, int n_args, ...) {
  // list of extra args passed to function
  va_list args;
  // number of extra args
  va_start(args, n_args);

  // create node
  node_t *new_node = create_node(type, value);

  // iterate extra args
  for (int i = 0; i < n_args; i++) {
    node_t *n = va_arg(args, node_t *);

    if (n != NULL) {
      printf("INSERTING NEW CHILD\n");

      if (new_node->child == NULL) {
        new_node->child = n;
      }

      else {
        node_t *aux = new_node;
        while (aux->brother != NULL) {
          aux = aux->brother;
        }
        aux->brother = n;
      }
    }
  }

  va_end(args);
  return new_node;
}
