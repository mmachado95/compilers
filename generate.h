#include <stdio.h>
#include "ast.h"
#include "symbol_table.h"


void generate_code(node_t *ast);
void generate_code_program(node_t *ast);
void generate_code_declaration(node_t *ast);
void generate_code_func_declaration(node_t *ast);
void generate_code_func_definition(node_t *ast);
void generate_code_assign_operator(node_t *ast);
void generate_code_arithmetic_operator(node_t *ast);
void generate_code_unary_operator(node_t *ast);
void generate_code_terminal(node_t *ast);

void print_param_types(param_type *params);
void print_param_types_and_ids(node_t *params);

char *get_llvm_type(char *type_name);
