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

  // create node
  node_t *new_node = create_node(type, value);
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
  node_t *aux = original;

  while (aux->sibling != NULL) {
    aux = aux->sibling;
  }

  aux->sibling = sibling;
  //printf("Adding sibling to node %s!!\n", original->type);
  //printf("-> %s!!\n", sibling->type);

  return original;
}

node_t *make_node_correct(node_t *node_to_correct) {
  if (node_to_correct == NULL) {
    node_to_correct = insert_node("Null", NULL, 0);
  }
  return node_to_correct;
}

void insert_node_special(node_t *first, node_t *second) {
  node_t *n = second;
  while(n != NULL){
    node_t *t = insert_node(first->type, NULL, 0);
    t = add_sibling(t, n->child);
    n->child = t;
    n = n->sibling;
  }
}

void print_ast(node_t *n, int depth){
  if (n != NULL) {
    for(int i = 0; i < depth; i++)
      printf("..");

    if(n->value != NULL){
      printf("%s(%s)\n", n->type,n->value);
    } else{
      printf("%s\n", n->type);
    }

    if(n->child != NULL) {
      print_ast(n->child, depth + 1);
    }
    if(n->sibling != NULL) {
      print_ast(n->sibling, depth);
    }
  }
}
