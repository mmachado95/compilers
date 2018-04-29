#ifndef SEMANTICS_HEADER
#define SEMANTICS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"

// =========== MAIN FUNCTIONS =============
void check_program(node_t *program);
void check_declaration(node_t *declaration);
void check_func_declaration(node_t *func_declaration);
void check_func_definition(node_t *func_definition);
void check_param_list(node_t *param_list, symbol *func_declaration, int is_func_def); //param declaration is apart of param_list
void check_func_body(node_t *func_body);
void check_comma(node_t *comma);
//void check_if(node_t *if_);
//void check_while(node_t *while_);
void check_return(node_t *return_);
void check_arithmetic_operator(node_t *operator_);
void check_unary_operator(node_t *operator_);
void check_relational_operator(node_t *operator_);
void check_logical_operator(node_t *operator_);
void check_bitwise_operator(node_t *operator_);
void check_assign_operator(node_t *operator_);
void check_call(node_t *operator_);
void check_terminal(node_t *terminal);

#endif
