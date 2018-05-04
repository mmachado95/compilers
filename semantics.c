#include <ctype.h>
#include "semantics.h"

void check_program(node_t *ast) {

  // stop condition
  if (ast == NULL) {
    return;
  }

  if (strcmp(ast->type, "Program") == 0) {
    check_program(ast->child);
  }
  else if (strcmp(ast->type, "Declaration") == 0) {
    check_declaration(ast);
  }
  else if (strcmp(ast->type, "FuncDeclaration") == 0) {
    check_func_declaration(ast);

    // doesn't expand tree if func has error
    if (ast->has_error == 1) {
      check_program(ast->sibling);
      return;
    }
  }
  else if (strcmp(ast->type, "FuncDefinition") == 0) {
    check_func_definition(ast);
    current = tables;

    // doesn't expand tree if func has error
    if (ast->has_error == 1) {
      check_program(ast->sibling);
      return;
    }
  }
  else if (strcmp(ast->type, "FuncBody") == 0) {
    check_program(ast->child);
    current = tables;
  }
  else if (strcmp(ast->type, "Comma") == 0) {
    check_comma(ast);
  }
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
  check_program(aux->sibling->sibling);

  if (strcmp(aux->type, "void") == 0 || strcmp(aux->type, "Void") == 0) {
    printf("Line %d, col %d: Invalid use of void type in declaration\n", aux->sibling->line, aux->sibling->col);
  } else {
    if (get_element(current, aux->sibling->value) == NULL) {
      insert_element(current, aux->sibling->value, aux->type, NULL);
    } else if (strcmp("Global", current->name) != 0) {
      printf("Line %d, col %d: Symbol %s already defined\n", aux->sibling->line, aux->sibling->col, aux->sibling->value);
      return;
    }
  }

  if (aux->sibling->sibling != NULL) {
    char *aux_name = strdup(aux->sibling->sibling->type_e);
    char *aux2_name = strdup(aux->type);
    aux_name[0] = tolower(aux_name[0]);
    aux2_name[0] = tolower(aux2_name[0]);

    int score_aux;
    int score_aux2;

    if (strcmp(aux_name, "undef") == 0) {
      score_aux = 5;
    }
    if (strcmp(aux2_name, "undef") == 0) {
      score_aux2 = 5;
    }
    if (strcmp(aux_name, "double") == 0) {
      score_aux = 4;
    }
    if (strcmp(aux2_name, "double") == 0) {
      score_aux2 = 4;
    }
    if (strcmp(aux_name, "int") == 0) {
      score_aux = 3;
    }
    if (strcmp(aux2_name, "int") == 0) {
      score_aux2 = 3;
    }
    if (strcmp(aux_name, "short") == 0) {
      score_aux = 2;
    }
    if (strcmp(aux2_name, "short") == 0) {
      score_aux2 = 2;
    }
    if (strcmp(aux_name, "char") == 0) {
      score_aux = 3;
    }
    if (strcmp(aux2_name, "char") == 0) {
      score_aux2 = 3;
    }

    if (score_aux2 < score_aux)  {
      printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", aux->sibling->line, aux->sibling->col, aux_name, aux2_name);
    }
  }
}


void check_func_declaration(node_t *func_declaration) {
  // save type of function
  char *func_type = func_declaration->child->type;

  // go to FunctionDeclarator
  node_t *aux = func_declaration->child->sibling;
  char *func_name = aux->value;

  int void_error = check_void_error(func_declaration, aux->sibling, NULL);
  if (void_error == 0) {

    // if table for function doesn't exist already
    if (get_table(aux->value) == NULL) {
      // create table for function
      current = create_table(func_name, 0);

      // insert element in global table
      symbol *func_declaration_sym = insert_element(tables, func_name, func_type, NULL);

      // add return statement to table
      insert_element(current, "return", func_type, NULL);

      check_param_names(func_declaration);

      // function that adds the param types to the symbol
      check_param_list(func_declaration, aux->sibling, func_declaration_sym, 0);
      check_void_error(func_declaration, aux->sibling, func_declaration_sym);
    }
    else {
      symbol *prev_func_declaration = get_element(tables, func_name);

      char *prev_func_declaration_type = strdup(prev_func_declaration->type);
      prev_func_declaration_type[0] = tolower(prev_func_declaration_type[0]);
      char *func_declaration_type = strdup(func_declaration->child->type);
      func_declaration_type[0] = tolower(func_declaration_type[0]);

      // Error - Conflicting types
      if (strcmp(prev_func_declaration_type, func_declaration_type) != 0 || conflicting_types_params(prev_func_declaration, func_declaration) == 1) {
        printf("Line %d, col %d: Conflicting types (got ", aux->line, aux->col);
        printf("%s(", func_declaration_type);

        node_t *aux_node = func_declaration->child->sibling->sibling->child;
        int index = 0;
        while (aux_node != NULL) {
          char *aux_node_child_type = strdup(aux_node->child->type);
          aux_node_child_type[0] = tolower(aux_node_child_type[0]);
          if (index == 0) {
            printf("%s", aux_node_child_type);
            index++;
          }
          else {
            printf(",%s", aux_node_child_type);
          }
          aux_node = aux_node->sibling;
          free(aux_node_child_type);
        }

        printf("), expected %s(", prev_func_declaration_type);
        show_func_param_types(prev_func_declaration->param);

        free(func_declaration_type);
        free(prev_func_declaration_type);

        printf("))\n");
      }
    }

  }

  current = tables;
}

void check_func_definition(node_t *func_definition) {
  char *func_type = func_definition->child->type;

  // go to FunctionDeclarator
  node_t *aux = func_definition->child->sibling;
  char *func_name = aux->value;
  symbol *func;
  int void_error = check_void_error(func_definition, aux->sibling, NULL);

  if (void_error == 0) {
    current = get_table(func_name);

    // If function was not declared or defined
    if(current == NULL) {
      current = create_table(func_name, 1);
      current->print = 1;

      // insert function in global table
      func = insert_element(tables, func_name, func_type, NULL);

      // add return statement to table
      insert_element(current, "return", func_type, NULL);

      check_param_list(func_definition, aux->sibling, func, 0);
      check_param_list(func_definition, aux->sibling, func, 1);
      check_param_names(func_definition);
    }

    // Function has been declared or defined
    else {
      func = get_element(tables, func_name);
      int declaration_has_error = func->has_error;

      if (declaration_has_error == 0) {
        func->to_print = 1;
      }

      // Function has been defined
      // Error - symbol already defined
      table *aux_tables = tables;
      while (aux_tables != NULL) {
        if (strcmp(func_name, aux_tables->name) == 0 && aux_tables->is_func_definition == 1) {
          printf("Line %d, col %d: Symbol %s already defined\n", aux->line, aux->col, aux->value);
          func_definition->has_error = 1;
          break;
        }
        aux_tables = aux_tables->next;
      }


      if (func_definition->has_error == 0) {
        current->print = 1;

        char *func_declaration_type = strdup(func->type);
        func_declaration_type[0] = tolower(func_declaration_type[0]);
        char *func_definition_type = strdup(func_definition->child->type);
        func_definition_type[0] = tolower(func_definition_type[0]);

        // Error - Conflicting types
        if (strcmp(func_declaration_type, func_definition_type) != 0 || conflicting_types_params(func, func_definition) == 1) {
          current->print = 0;
          func_definition->has_error = 1;
          printf("Line %d, col %d: Conflicting types (got ", aux->line, aux->col);
          printf("%s(", func_definition_type);

          node_t *aux_node = func_definition->child->sibling->sibling->child;
          int index = 0;
          while (aux_node != NULL) {
            char *aux_node_child_type = strdup(aux_node->child->type);
            aux_node_child_type[0] = tolower(aux_node_child_type[0]);
            if (index == 0) {
              printf("%s", aux_node_child_type);
              index++;
            }
            else {
              printf(",%s", aux_node_child_type);
            }
            aux_node = aux_node->sibling;
            free(aux_node_child_type);
          }

          printf("), expected %s(", func_declaration_type);
          show_func_param_types(func->param);

          free(func_definition_type);
          free(func_declaration_type);

          printf("))\n");
        }
        else {
          check_param_list(func_definition, aux->sibling, func, 1);
          current->is_func_definition = 1;
        }
      }
      check_param_names(func_definition);
    }

    if (func_definition->has_error == 0) {
      check_program(aux->sibling->sibling);
    }
  }
}


int check_void_error(node_t *func_node, node_t *param_list, symbol *func) {
  node_t *param_list_aux = param_list->child;
  node_t *param_void_error = NULL;
  int number_of_params = 0;
  int has_void_param = 0;

  if (strcmp("Void", param_list_aux->child->type) == 0 && param_list_aux->child->sibling != NULL) {
    printf("Line %d, col %d: Invalid use of void type in declaration\n", param_list_aux->child->line, param_list_aux->child->col);
    return 1;
  }

  // get types of params
  while(param_list_aux != NULL) {
    char *param_type = param_list_aux->child->type;
    number_of_params++;

    // if there is a void and it's not isolated
    if (param_void_error == NULL && strcmp("Void", param_type) == 0) {
      param_void_error = param_list_aux->child;
    }

    param_list_aux = param_list_aux->sibling;
  }

  if(number_of_params <= 1 && has_void_param == 0) {
    param_void_error = NULL;
  }

  if (param_void_error != NULL) {
    if (func != NULL) {
      current->print = 0;
      func_node->has_error = 1;
      func->has_error = 1;
      func->to_print = 0;
    }
    printf("Line %d, col %d: Invalid use of void type in declaration\n", param_void_error->line, param_void_error->col);
    return 1;
  }

  return 0;
}


void check_param_list(node_t *func_node, node_t *param_list, symbol *func, int is_func_def) {
  node_t *param_list_aux = param_list->child;

  // get types of params
  while(param_list_aux != NULL) {
    char *param_type = param_list_aux->child->type;

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
    }

    else {
      insert_type(param_type, func);
    }

    param_list_aux = param_list_aux->sibling;
  }
}


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
}


void check_relational_operator(node_t *operator_) {
  check_program(operator_->child);
  operator_->type_e = strdup("int");
}


void check_bitwise_operator(node_t *operator_) {
  check_program(operator_->child);

  if (strcmp(operator_->child->type_e, "double") == 0 || strcmp(operator_->child->sibling->type_e, "double") == 0 || strcmp(operator_->child->type_e, "Double") == 0 || strcmp(operator_->child->sibling->type_e, "Double") == 0) {
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
  else if (strcmp(operator_->child->type_e, "undef") == 0 || strcmp(operator_->child->sibling->type_e, "undef") == 0) {
    operator_->type_e = strdup("undef");
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

  if (strcmp(operator_->child->type_e, "double") == 0 || strcmp(operator_->child->sibling->type_e, "double") == 0 || strcmp(operator_->child->type_e, "Double") == 0 || strcmp(operator_->child->sibling->type_e, "Double") == 0) {
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
  else if (strcmp(operator_->child->type_e, "undef") == 0 || strcmp(operator_->child->sibling->type_e, "undef") == 0) {
    operator_->type_e = strdup("undef");
  }
}


void check_terminal(node_t *terminal) {
  if (strcmp(terminal->type, "Id") == 0) {
    symbol *global_id = get_element(tables, terminal->value);
    symbol *local_id = get_element(current, terminal->value);

    if(local_id != NULL && local_id->has_error == 0) {
      terminal->type_e = local_id->type;
    }
    else if (global_id != NULL && global_id->has_error == 0) {
      terminal->type_e = global_id->type;
    }
    else {
      printf("Line %d, col %d: Unknown symbol %s\n", terminal->line, terminal->col, terminal->value);
      terminal->type_e = strdup("undef");
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


int check_conflicting_types(char *got, char *expected, int line, int col) {
  if (strcmp(got, expected) == 0) {
    printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n", line, col, got, expected);
    return 1;
  }
  return 0;
}


int conflicting_types_params(symbol *func_declaration, node_t *func_definition) {
  param_type *aux_param = func_declaration->param;
  node_t *aux_node = func_definition->child->sibling->sibling->child;

  while (aux_param != NULL && aux_node != NULL) {
    char *aux_type = strdup(aux_node->child->type);
    aux_param->name[0] = tolower(aux_param->name[0]);
    aux_type[0] = tolower(aux_type[0]);

    if(strcmp(aux_param->name, aux_type) != 0) {
      return 1;
    }

    free(aux_type);

    aux_node = aux_node->sibling;
    aux_param = aux_param->next;

    if((aux_param != NULL && aux_node == NULL) || (aux_param == NULL && aux_node != NULL)) {
      return 1;
    }
  }

  return 0;
}


void check_param_names(node_t *func) {
  node_t *aux = func->child->sibling->sibling->child;
  node_t *aux2 = aux;

  while (aux != NULL) {
    if (aux->child->sibling != NULL) {
      aux2 = aux->sibling;
      while (aux2 != NULL) {
        if (aux2->child->sibling != NULL) {
          if (strcmp(aux->child->sibling->value, aux2->child->sibling->value) == 0) {
            printf("Line %d, col %d: Symbol %s already defined\n", aux2->child->sibling->line, aux2->child->sibling->col, aux2->child->sibling->value);
            return;
          }
        }
        aux2 = aux2->sibling;
      }
    }
    aux = aux->sibling;
    aux2 = aux;
  }
}
