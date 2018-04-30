#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symbol_table.h"

// adds the default functions to the global table
void insert_default_functions(table *to_insert) {
  symbol *put_char = insert_element(to_insert, "putchar", "Int", NULL);
  insert_type("Int", put_char);
  symbol *get_char = insert_element(to_insert, "getchar", "Int", NULL);
  insert_type("Void", get_char);
}

table *create_table(char *name) {
  // creates the table and respective elements
  table *new_table = (table *)malloc(sizeof(table));
  new_table->print = 0;
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
  new_symbol->is_param = 0;
  new_symbol->name = NULL;
  if (name != NULL) {
    new_symbol->name = strdup(name);
  }
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

// aux function to show function param types
void show_func_param_types(param_type *param) {
  param_type *aux = param;
  aux->name[0] = tolower(aux->name[0]);
  printf("%s", aux->name);
  
  aux = aux->next;

  while(aux != NULL) {
    printf(",");
    aux->name[0] = tolower(aux->name[0]);
    printf("%s", aux->name);
    aux = aux->next;
  }
}

void show_symbol(symbol *symbol) {
  symbol->type[0] = tolower(symbol->type[0]);
  printf("%s\t%s", symbol->name, symbol->type);

  if (symbol->is_param == 1) {
    printf("\tparam\n");
  }
  else if (symbol->param != NULL) {
    printf("(");
    show_func_param_types(symbol->param);
    printf(")\n");
  } else {
    printf("\n");
  }
}

void show_table(table *table) {
  symbol *aux = table->symbol;

  // check if it's global function or not
  if(strcmp("Global", table->name) != 0) {
    printf("===== Function %s Symbol Table =====\n", table->name);
  } else {
    printf("===== %s Symbol Table =====\n", table->name);
  }

  while(aux != NULL) {
    show_symbol(aux);
    aux = aux->next;
  }
}

void show_tables() {
  table *aux = tables;

  while(aux != NULL) {
    if(aux->print) {
      show_table(aux);
      printf("\n");
    }
    aux = aux->next;
  }
}
