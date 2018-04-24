#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pt_ {
  // name of param type
  char *name;
  struct pt_ *next;
} param_type;

typedef struct sym {
  // identifier of variable, function etc
  char *name;
  // type of variable, function etc
  char *type;
  // if symbol is a function we need to know the types of the params
  param_type *param;
  struct sym *next;
} symbol;

typedef struct tab {
  // name of table
  char *name;
  // symbols that table has
  symbol *symbol;
  struct tab *next;
} table;

void insert_default_functions(table *to_insert);
table *create_table(char *name);
table *get_table(char *name);
symbol *insert_element(table *table, char *name, char *type, param_type *params_types);
symbol *get_element(table *table, char *name);
void insert_type(char *name, symbol *to_insert_type);
void show_tables();


table *tables;
table *current;

#endif
