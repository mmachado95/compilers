#ifndef SEMANTICS_HEADER
#define SEMANTICS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"

extern int line;
extern int col;

void check_program(node_t *program);
void check_declaration(node_t *declaration);
void check_func_declaration(node_t *func_declaration);
void check_func_definition(node_t *func_definition);
void check_param_list(node_t *func_node, node_t *param_list, symbol *func, int is_func_def);
int check_void_error(node_t *func_node, node_t *param_list, symbol *func);
void check_func_body(node_t *func_body);
void check_comma(node_t *comma);
void check_return(node_t *return_);
void check_arithmetic_operator(node_t *operator_);
void check_unary_operator(node_t *operator_);
void check_relational_operator(node_t *operator_);
void check_logical_operator(node_t *operator_);
void check_bitwise_operator(node_t *operator_);
void check_assign_operator(node_t *operator_);
void check_call(node_t *operator_);
void check_terminal(node_t *terminal);
int conflicting_types_params(symbol *func_declaration, node_t *func_definition);
void check_param_names(node_t *func);

#endif
