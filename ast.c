#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "ast.h"

node_t* create_node(int line, int col, char *type, char *value) {
  // allocate memory for new node
  node_t *n = (node_t*) malloc(sizeof (node_t));

  n->has_error = 0;

  // save line and col of symbol
  n->line = line;
  n->col = col;

  // save strings (save lines by using strdup)
  n->type = strdup(type);
  if (value == NULL) {
    n->value = value;
  } else {
    n->value = strdup(value);
  }

  n->type_e = NULL;
  n->child = NULL;
  n->sibling = NULL;
  return n;
}

// insert node in ast
node_t* insert_node(int line, int col, char *type, char *value, int n_args, ...) {
  int i;

  // list of extra args passed to function
  va_list args;
  // number of extra args
  va_start(args, n_args);

  // create node
  node_t *new_node = create_node(line, col, type, value);
  //printf("New node -> %s %s\n", new_node->type, new_node->value);
  node_t *aux;

  // iterate extra args
  for (i = 0; i < n_args; i++) {
    node_t *n = va_arg(args, node_t *);

    if (n != NULL) {
      //printf("Adding child to node %s -> %s!!\n", new_node->type, n->type);
      if (new_node->child == NULL) {
        new_node->child = n;
        aux = n;
      }
      else {
        add_sibling(aux, n);
        aux = n;
      }
    }
  }

  va_end(args);
  return new_node;
}

// adds sibling to node
node_t *add_sibling(node_t *original, node_t *sibling) {

  if (original == NULL) {
    return sibling;
  }
  if (sibling == NULL) {
    return original;
  }
  if (original && sibling == NULL) {
    return NULL;
  }

  node_t *aux = original;

  while (aux->sibling != NULL) {
    aux = aux->sibling;
  }

  aux->sibling = sibling;

  return original;
}

node_t *make_node_correct(int line, int col, node_t *node_to_correct) {
  if (node_to_correct == NULL) {
    node_to_correct = insert_node(line, col, "Null", NULL, 0);
  }
  return node_to_correct;
}

void insert_node_special(node_t *first, node_t *second) {
  node_t *n = second;
  while(n != NULL){
    node_t *t = insert_node(first->line, first->col, first->type, NULL, 0);
    t = add_sibling(t, n->child);
    n->child = t;
    n = n->sibling;
  }
}

void print_ast(node_t *n, int depth){
  for(int i = 0; i < depth; i++)
    printf("..");

  if(n->value != NULL){
    printf("%s(%s)", n->type, n->value);
  }
  else {
    printf("%s", n->type);
  }

  // Annotated AST
  if (n->type_e != NULL) {
    printf(" - %s", n->type_e );

    if (n->value != NULL && strcmp("undef", n->type_e) != 0) {
      table *global_table = get_table("Global");
      symbol *symbol = get_element(global_table, n->value);

      if (symbol != NULL && symbol->has_error == 0) {
        if (symbol->is_param == 1) {
          printf("\tparam");
        }
        else if (symbol->param != NULL) {
          printf("(");
          show_func_param_types(symbol->param);
          printf(")");
        }
      }
    }
  }

  printf("\n");

  if(n->child != NULL) {
    print_ast(n->child, depth + 1);
  }
  if(n->sibling != NULL) {
    print_ast(n->sibling, depth);
  }
}

void destroy_ast(node_t *current) {
  if (current == NULL) {
    return;
  }
  if (current->type != NULL) {
    free(current->type);
  }
  if (current->value != NULL) {
    free(current->value);
  }

  destroy_ast(current->child);
  destroy_ast(current->sibling);
  free(current);
}
