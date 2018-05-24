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
  // used to know if function has error or not. If function has an error we give the terminal type undef
  int has_error;
  // used to know if function is supposed to print in the global table
  int to_print;
  // if the symbol is a param we need to print param at the end
  int is_param;
  // identifier of variable, function etc
  char *name;
  // type of variable, function etc
  char *type;
  // if symbol is a function we need to know the types of the params
  param_type *param;
  struct sym *next;
} symbol;

typedef struct tab {
  int is_func_definition;
  // table is supposed to be printed
  int print;
  // name of table
  char *name;
  // symbols that table has
  symbol *symbol;
  struct tab *next;
} table;

void insert_default_functions(table *to_insert);
table *create_table(char *name, int is_func_definition);
table *get_table(char *name);
symbol *insert_element(table *table, char *name, char *type, param_type *params_types);
symbol *get_element(table *table, char *name);
void insert_type(char *name, symbol *to_insert_type);
void show_func_param_types(param_type *param);
void show_symbol(symbol *symbol);
void show_table(table *table);
void show_tables();

void destroy_param_types(param_type *param);
void destroy_symbols(symbol *symbols);
void destroy_tables();

table *tables;
table *current;

#endif
