
#include <stdarg.h>
#include <string.h>
#include "ast.h"

node_t* insert_to_ast(char *node_name, int n_args, ... ) {
  va_list children;
  va_start(children, n_args);

  node_t *n;
  n = (node_t*) malloc(sizeof (node_t));
  n->name = node_name;
  n->children = NULL;
  n->brother = NULL;

  for (int i = 0; i < n_args; i++) {
    node_t *child = va_arg(children, node_t*);

    if (child != NULL) {
      printf("NEW CHILD\n");
    }
  }

  return n;
}
