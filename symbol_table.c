#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

// util function to add a symbol to the end of a table
void add_symbol_to_table_end(table *to_insert, symbol *new_symbol) {

  while(to_insert->next != NULL) {
    to_insert = to_insert->next;
  }

  to_insert->next = to_insert;
}

// ================================================
// ==================== MAIN ======================
// ================================================

table *create_table(char *name) {
  // creates the table and respective elements
  table *new_table = (table *)malloc(sizeof(table));
  new_table->name = strdup(name);
  new_table->symbol = NULL;
  new_table->next = NULL;

  // go to end of tables list
  table *aux = tables;
  if(tables != NULL) {
    while(aux->next != NULL) {
      // check if table already exists
      if(strcmp(name, aux->name) == 0) {
        return NULL;
      }

      aux = aux->next;
    } 
  }
  // add to end of list
  aux->next = new_table;

  return new_table;
}

// search table by its name
table *get_table(char *name) {
  table *search_aux = tables;

  while(search_aux != NULL) {
    if(strcmp(name, search_aux->name) == 0) {
      return search_aux;
    }

    search_aux = search_aux->next;
  }

  return NULL;
}

//Insere um novo identificador na cauda de uma lista ligada de simbolo
symbol *insert_element(table *to_insert, char *name, char *type) {
  symbol *new_symbol=(symbol *) malloc(sizeof(symbol));
  new_symbol->name = strdup(name);
  new_symbol->type = strdup(type);
  new_symbol->next = NULL;

  symbol *aux = to_insert->symbol;

  // check if table already has symbols
  if (aux != NULL) {
    while(aux->next != NULL) {
      aux = aux->next;
    }
    aux->next = new_symbol;
  } else {
    aux->next = new_symbol;
  }

  return new_symbol; 
}

//return table symbol with the same name
symbol *get_element(table *table, char *name) {
  symbol *aux = table->symbol;

  while(aux != NULL) {
    if (strcmp(name, aux->name) == 0) {
      return aux;
    }

    aux = aux->next;
  }

  return NULL;
}
