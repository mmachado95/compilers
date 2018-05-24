#include <stdio.h>
#include "ast.h"

void generate_code(node_t *ast);
void generate_code_program(node_t *ast);
void generate_code_func_declaration(node_t *ast);

char *get_type(char *type_name);
