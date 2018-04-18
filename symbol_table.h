#include <stdio.h>
#include <stdlib.h>

typedef struct sym {
  char *name;
  char *type;
  struct sym *next;
} symbol;

typedef struct tab {
  char *name;
  symbol *symbol;
  struct tab *next;
} table;

table *create_table(char *name);
table *get_table(char *name);
symbol *insert_element(table *table, char *name, char *type);
symbol *get_element(table *table, char *name);
void show_table();


table *tables;
