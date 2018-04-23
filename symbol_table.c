#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

    // add to end of list
    aux->next = new_table;
  } else {
    tables = new_table;
  }

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

// insert new symbol at table end
symbol *insert_element(table *to_insert, char *name, char *type, param_type *params_types) {

  // create the symbol
  symbol *new_symbol=(symbol *) malloc(sizeof(symbol));
  new_symbol->name = strdup(name);
  new_symbol->type = strdup(type);
  new_symbol->param = params_types;
  new_symbol->next = NULL;

  // create pointer to the first symbol in table
  symbol *aux = to_insert->symbol;

  // iterate symbols of table
  if (aux != NULL) {
    while(aux->next != NULL) {
      aux = aux->next;
    }
    aux->next = new_symbol;
  } else {
    to_insert->symbol = new_symbol;
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

void insert_type(char *name, symbol *to_insert_type) {
  param_type *type_aux = to_insert_type->param;

  param_type *type = (param_type *) malloc(sizeof(param_type));
  type->name = strdup(name);
  type->next = NULL;

  if (type_aux != NULL) {
    while(type_aux->next != NULL) {
      type_aux = type_aux->next;
    }

    type_aux->next = type;
  } else {
    to_insert_type->param = type;
  }
}

void show_tables() {
  table *aux = tables;

  while(aux != NULL) {
    if(strcmp("Global", aux->name) != 0) {
      printf("===== Function %s Symbol Table =====\n", aux->name);
    } else {
      printf("===== %s Symbol Table =====\n", aux->name);
    }

    symbol *aux2 = aux->symbol;

    while(aux2 != NULL) {
      if(aux2->type != NULL) {
        aux2->type[0] = aux2->type[0] + 32; // Lower Case
        printf("%s\t%s(", aux2->name, aux2->type);

        param_type *param_aux = aux2->param;
        int i = 0;
        while(param_aux != NULL) {
          param_aux->name[0] = param_aux->name[0] + 32; // Lower Case
          if (i == 0) {
            printf("%s", param_aux->name);
            i++;
          }
          else {
            printf(",%s", param_aux->name);
          }
          param_aux = param_aux->next;
        }
        printf(")\n");
      }

      else {
        printf("%s\n", aux2->name);
      }

      aux2 = aux2->next;
    }

    aux=aux->next;
  }
}
