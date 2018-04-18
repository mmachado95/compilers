#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

void check_program(node_t *program);
void check_declaration(node_t *declaration);
void check_func_declaration(node_t *func_declaration);
void check_func_definition(node_t *func_definition);
void check_param_list(node_t *param_list);
void check_func_body(node_t *func_body);
void check_param_declaration(node_t *param_declaration);
void check_if(node_t *if_);
void check_while(node_t *while_);
void check_return(node_t *return_);
void check_operator(node_t *operator_);
void check_terminals(node_t *terminals);
void check_special(node_t *special);
