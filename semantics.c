#include "semantics.h"

void check_program(node_t *ast) {

  // stop condition
  if (ast == NULL) {
    return;
  }

  // printf("%s\n", ast->type );

  if (strcmp(ast->type, "Program") == 0) {
    check_program(ast->child);
  }
  else if (strcmp(ast->type, "Declaration") == 0) {
    check_declaration(ast);
  }
  else if (strcmp(ast->type, "FuncDeclaration") == 0) {
    check_func_declaration(ast);
  }
  else if (strcmp(ast->type, "FuncDefinition") == 0) {
    check_func_definition(ast);
    current = tables;
  }
  // TODO check if func body has child or not
  else if (strcmp(ast->type, "FuncBody") == 0) {
    check_program(ast->child);
    current = tables;
  }
  else if (strcmp(ast->type, "Comma") == 0) {
    check_comma(ast);
  }
  /*
  else if (strcmp(ast->type, "If") == 0) {
    //check_if(ast);
  }
  else if (strcmp(ast->type, "While") == 0) {
    //check_while(ast);
  } */
  else if (strcmp(ast->type, "Return") == 0) {
    check_return(ast);
  }
  else if (strcmp(ast->type, "Store") == 0) {
    check_assign_operator(ast);
  }
  else if (strcmp(ast->type, "Add") == 0
      || strcmp(ast->type, "Sub") == 0
      || strcmp(ast->type, "Mul") == 0
      || strcmp(ast->type, "Div") == 0) {
    check_arithmetic_operator(ast);
  }
  else if (strcmp(ast->type, "Plus") == 0
      || strcmp(ast->type, "Minus") == 0) {
    check_unary_operator(ast);
  }
  else if (strcmp(ast->type, "Eq") == 0
      || strcmp(ast->type, "Ne") == 0
      || strcmp(ast->type, "Le") == 0
      || strcmp(ast->type, "Ge") == 0
      || strcmp(ast->type, "Lt") == 0
      || strcmp(ast->type, "Gt") == 0
      || strcmp(ast->type, "Mod") == 0)  {
    check_relational_operator(ast);
  }
  else if (strcmp(ast->type, "Not") == 0
      || strcmp(ast->type, "And") == 0
      || strcmp(ast->type, "Or") == 0) {
    check_logical_operator(ast);
  }
  else if (strcmp(ast->type, "BitWiseAnd") == 0
      || strcmp(ast->type, "BitWiseOr") == 0
      || strcmp(ast->type, "BitWiseXor") == 0) {
    check_bitwise_operator(ast);
  }
  else if (strcmp(ast->type, "Call") == 0) {
    check_call(ast);
  }
  else if (strcmp(ast->type, "Id") == 0
      || strcmp(ast->type, "IntLit") == 0
      || strcmp(ast->type, "ChrLit") == 0
      || strcmp(ast->type, "RealLit") == 0) {
    check_terminal(ast);
  }
  else {
    check_program(ast->child);
  }

  check_program(ast->sibling);
}


void check_declaration(node_t *declaration) {
  node_t *aux = declaration->child;

  if (strcmp(aux->type, "void") == 0 || strcmp(aux->type, "Void") == 0) {
    // Error - invalid use of void type in declaration
    printf("Line %d, col %d: Invalid use of void type in declaration\n", aux->sibling->line, aux->sibling->col);
  }

  // if symbol is not in table, add to table
  if (get_element(current, aux->sibling->value) == NULL) {
    insert_element(current, aux->sibling->value, aux->type, NULL);
  } else if (strcmp("Global", current->name) != 0) {
    // Error - symbol already defined
    printf("Line %d, col %d: Symbol %s already defined\n", aux->sibling->line, aux->sibling->col, aux->sibling->value);
  }

  if (aux->sibling->sibling != NULL) { //safe check, not sure if needed
    check_program(aux->sibling->sibling);
  }
}


void check_func_declaration(node_t *func_declaration) {
  // save type of function
  char *func_type = func_declaration->child->type;

  // go to FunctionDeclarator
  node_t *aux = func_declaration->child->sibling;

  // if table for function doesn't exist already
  if (get_table(aux->value) == NULL) {
    char *func_name = aux->value;
    // create table for function
    current = create_table(func_name);

    // insert element in global table
    symbol *func_declaration = insert_element(tables, func_name, func_type, NULL);

    // function that adds the param types to the symbol
    check_param_list(aux->sibling, func_declaration, 0, 1);
  }

  current = tables;
}

void check_func_definition(node_t *func_definition) {
  char *func_type = func_definition->child->type;

  //go to FunctionDeclarator
  node_t *aux = func_definition->child->sibling;

  // save function name
  char *func_name = aux->value;

  // pointer to function symbol
  symbol *func;

  // if table for function doesn't exist we need to create it
  current = get_table(func_name);
  if(current == NULL) {
    // function is defined, we need to print
    current = create_table(func_name);
    current->print = 1;

    // insert function in global table
    func = insert_element(tables, func_name, func_type, NULL);

    // add functions param types to global
    check_param_list(aux->sibling, func, 0, 1);
  } else {
    // function is defined, we need to print
    func = get_element(tables, func_name);
    current->print = 1;
  }

  // add return statement to table
  insert_element(current, "return", func_type, NULL);

  // add param to table function
  check_param_list(aux->sibling, func, 1, 0);

  check_program(aux->sibling->sibling);

}

void check_param_list(node_t *param_list, symbol *func, int is_func_def, int print_error) {
  node_t *param_list_aux = param_list->child;

  // vars used to help check foi void error
  node_t *param_void_error = NULL;
  int number_of_params = 0;
  int has_void_param = 0;


  // get types of params
  while(param_list_aux != NULL) {
    char *param_type = param_list_aux->child->type;
    number_of_params++;

    // if there is a void and its not isolated
    if (strcmp("Void", param_type) == 0) {
      param_void_error = param_list_aux->child;
    }

    // go to function id
    node_t *param_declaration = param_list_aux->child->sibling;
    if(param_declaration != NULL && strcmp("Id", param_declaration->type) != 0) {
      param_declaration = param_declaration->sibling;
    }

    // check if it's a function definition
    if(is_func_def == 1) {
      if(param_declaration != NULL && get_element(current, param_declaration->value) == NULL) {
        symbol *new_symbol = insert_element(current, param_declaration->value, param_type, NULL);
        new_symbol->is_param = 1;
      }
    } else {
      insert_type(param_type, func);
    }

    param_list_aux = param_list_aux->sibling;
  }

  if(number_of_params <= 1 && has_void_param == 0) {
    param_void_error = NULL;
  }

  if (print_error == 1 && param_void_error != NULL) {
    current->print = 0;
    func->to_print = 0;
    printf("Line %d, col %d: Invalid use of void type in declaration\n", param_void_error->line, param_void_error->col);
  }
}


// Not sure
void check_comma(node_t *comma) {
  check_program(comma->child);
  comma->type_e = comma->child->sibling->type_e;
}


void check_assign_operator(node_t *operator_) {
  check_program(operator_->child);
  operator_->type_e = operator_->child->type_e;
}


void check_call(node_t *operator_) {
  check_program(operator_->child);
  operator_->type_e = operator_->child->type_e;

  if (strcmp(operator_->child->type_e, "undef") == 0) {
    // Error - Symbol is not a function - TODO -> check this
    // printf("Line %d, col %d: Symbol %s is not a function\n", operator_->line, operator_->col, operator_->child->value);
  }

  else {
    int number_of_args_required = 0;
    table *global_table = get_table("Global");
    symbol *symbol = get_element(global_table, operator_->child->value);

    int param_not_void = 1;

    if (symbol != NULL && param_not_void == 1) {
      if (symbol->param != NULL && param_not_void == 1) {
        if (strcmp(symbol->param->name, "void") == 0 || strcmp(symbol->param->name, "Void") == 0) {
          number_of_args_required = 0;
          param_not_void = 0;
        }
        if (param_not_void == 1) {
          param_type *aux = symbol->param;
          while(aux != NULL) {
            number_of_args_required++;
            aux = aux->next;
          }
        }
      }
    }

    int number_of_args_provided = 0;
    node_t *aux = operator_->child->sibling;
    while (aux != NULL) {
      number_of_args_provided++;
      aux = aux->sibling;
    }
    if (number_of_args_required != number_of_args_provided) {
      // Error - Wrong number of arguments
      printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n", operator_->child->line, operator_->child->col, operator_->child->value, number_of_args_provided, number_of_args_required);
    }


    /* Comment because WA
    else {

      if (symbol != NULL && param_not_void == 1) {
        if (symbol->param != NULL && param_not_void == 1) {
          if (strcmp(symbol->param->name, "void") == 0 || strcmp(symbol->param->name, "Void") == 0) {
            number_of_args_required = 0;
            param_not_void = 0;
          }

          if (param_not_void == 1) {
            param_type *aux = symbol->param;
            node_t *aux2 = operator_->child->sibling;

            while(aux != NULL && aux2 != NULL) {
              //printf("%s %s %s \n", symbol->name, aux->name, aux2->type_e);
              // TODO -> check if we can use strcasecmp
              if (strcasecmp(aux->name, aux2->type_e) == 0) {
                aux = aux->next;
                aux2 = aux2->sibling;
              }
              else {
                // Error - Operator cannot be applied to
                if (number_of_args_provided > 1) {
                  printf("Operator %s cannot be applied to types", symbol->name);
                }
                else {
                  printf("Operator %s cannot be applied to type", symbol->name);
                }
                node_t *aux = operator_->child->sibling;
                int index = 0;
                while (aux != NULL) {
                  if (index == 0) {
                    printf(" %s", aux->type_e);
                    index++;
                  }
                  else {
                    printf(", %s", aux->type_e);
                  }
                  aux = aux->sibling;
                }
                printf("\n");
                return;
              }
            }
          }
        }
      }
    }*/
  }
}


void check_relational_operator(node_t *operator_) {
  check_program(operator_->child);
  operator_->type_e = strdup("int");
}


void check_bitwise_operator(node_t *operator_) {
  check_program(operator_->child);

  // ORDER: double, int, short, char
  if (strcmp(operator_->child->type_e, "undef") == 0 || strcmp(operator_->child->sibling->type_e, "undef") == 0) {
    operator_->type_e = strdup("undef");
  }
  else if (strcmp(operator_->child->type_e, "double") == 0 || strcmp(operator_->child->sibling->type_e, "double") == 0 || strcmp(operator_->child->type_e, "Double") == 0 || strcmp(operator_->child->sibling->type_e, "Double") == 0) {
    operator_->type_e = strdup("double");
  }
  else if (strcmp(operator_->child->type_e, "int") == 0 || strcmp(operator_->child->sibling->type_e, "int") == 0 || strcmp(operator_->child->type_e, "Int") == 0 || strcmp(operator_->child->sibling->type_e, "Int") == 0) {
    operator_->type_e = strdup("int");
  }
  else if (strcmp(operator_->child->type_e, "short") == 0 || strcmp(operator_->child->sibling->type_e, "short") == 0 || strcmp(operator_->child->type_e, "Short") == 0 || strcmp(operator_->child->sibling->type_e, "Short") == 0) {
    operator_->type_e = strdup("short");
  }
  else if (strcmp(operator_->child->type_e, "char") == 0 || strcmp(operator_->child->sibling->type_e, "char") == 0 || strcmp(operator_->child->type_e, "Char") == 0 || strcmp(operator_->child->sibling->type_e, "Char") == 0) {
    operator_->type_e = strdup("char");
  }
}


void check_unary_operator(node_t *operator_) {
  check_program(operator_->child);
  operator_->type_e = operator_->child->type_e;
}


void check_logical_operator(node_t *operator_) {
  check_program(operator_->child);
  operator_->type_e = strdup("int");
}


void check_return(node_t *return_) {
  check_program(return_->child);
}


void check_arithmetic_operator(node_t *operator_) {
  check_program(operator_->child);

  // ORDER: double, int, short, char
  if (strcmp(operator_->child->type_e, "undef") == 0 || strcmp(operator_->child->sibling->type_e, "undef") == 0) {
    operator_->type_e = strdup("undef");
  }
  else if (strcmp(operator_->child->type_e, "double") == 0 || strcmp(operator_->child->sibling->type_e, "double") == 0 || strcmp(operator_->child->type_e, "Double") == 0 || strcmp(operator_->child->sibling->type_e, "Double") == 0) {
    operator_->type_e = strdup("double");
  }
  else if (strcmp(operator_->child->type_e, "int") == 0 || strcmp(operator_->child->sibling->type_e, "int") == 0 || strcmp(operator_->child->type_e, "Int") == 0 || strcmp(operator_->child->sibling->type_e, "Int") == 0) {
    operator_->type_e = strdup("int");
  }
  else if (strcmp(operator_->child->type_e, "short") == 0 || strcmp(operator_->child->sibling->type_e, "short") == 0 || strcmp(operator_->child->type_e, "Short") == 0 || strcmp(operator_->child->sibling->type_e, "Short") == 0) {
    operator_->type_e = strdup("short");
  }
  else if (strcmp(operator_->child->type_e, "char") == 0 || strcmp(operator_->child->sibling->type_e, "char") == 0 || strcmp(operator_->child->type_e, "Char") == 0 || strcmp(operator_->child->sibling->type_e, "Char") == 0) {
    operator_->type_e = strdup("char");
  }
}


void check_terminal(node_t *terminal) {
  if (strcmp(terminal->type, "Id") == 0) {
    symbol *id = get_element(current, terminal->value);

    if (id == NULL) {
      table *global_table = get_table("Global");
      id = get_element(global_table, terminal->value);
    }
    if (id == NULL) {
      // Error - Unknown symbol
      printf("Line %d, col %d: Unknown symbol %s\n", terminal->line, terminal->col, terminal->value);
      terminal->type_e = strdup("undef");
    }
    else {
      terminal->type_e = id->type;
    }
  }
  else if (strcmp(terminal->type, "IntLit") == 0) {
    terminal->type_e = strdup("int");
  }
  else if (strcmp(terminal->type, "ChrLit") == 0) {
    terminal->type_e = strdup("int");
  }
  else if (strcmp(terminal->type, "RealLit") == 0) {
    terminal->type_e = strdup("double");
  }
}
