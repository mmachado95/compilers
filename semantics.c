#include "semantics.h"

void check_program(node_t *ast) {

  // stop condition
  if (ast == NULL) {
    return;
  }

  printf("%s\n", ast->type );

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
  // TODO check if func body ahs child or not
  else if (strcmp(ast->type, "FuncBody") == 0) {
    check_program(ast->child);
    current = tables;
  }
  else if (strcmp(ast->type, "If") == 0) {
    //check_if(ast);
  }
  else if (strcmp(ast->type, "While") == 0) {
    //check_while(ast);
  }
  else if (strcmp(ast->type, "Return") == 0) {
    //check_return(ast);
  }
  else if (strcmp(ast->type, "Store") == 0) {
    //check_assign_operator(ast);
  }
  else if (strcmp(ast->type, "Add") == 0
      || strcmp(ast->type, "Sub") == 0
      || strcmp(ast->type, "Mul") == 0
      || strcmp(ast->type, "Div") == 0
      || strcmp(ast->type, "Mod") == 0) {
    //check_arithmetic_operator(ast);
  }
  else if (strcmp(ast->type, "Plus") == 0
      || strcmp(ast->type, "Minus") == 0) {
    //check_unary_operator(ast);
  }
  else if (strcmp(ast->type, "Eq") == 0
      || strcmp(ast->type, "Ne") == 0
      || strcmp(ast->type, "Le") == 0
      || strcmp(ast->type, "Ge") == 0
      || strcmp(ast->type, "Lt") == 0
      || strcmp(ast->type, "Gt") == 0
      || strcmp(ast->type, "Mod") == 0) {
    //check_relational_operator(ast);
  }
  else if (strcmp(ast->type, "Not") == 0
      || strcmp(ast->type, "And") == 0
      || strcmp(ast->type, "Or") == 0) {
    //check_logical_operator(ast);
  }
  else if (strcmp(ast->type, "BitWiseAnd") == 0
      || strcmp(ast->type, "BitWiseOr") == 0
      || strcmp(ast->type, "BitWiseXor") == 0) {
    //check_bitwise_operator(ast);
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

  check_program(ast->sibling);
}

void check_declaration(node_t *declaration) {
  node_t *aux = declaration->child;


  // if symbol is not in table, add to table
  if (get_element(current, aux->sibling->value) == NULL) {
    insert_element(current, aux->sibling->value, aux->type, NULL);
  } else {
    // should print an error of declaration already declared
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
    check_param_list(aux->sibling, func_declaration, 0);
  }

  current = tables;
}

void check_func_definition(node_t *func_definition) {
  char *func_type = func_definition->child->type;

  //go to FunctionDeclarator
  node_t *aux = func_definition->child->sibling;

  // save function name
  char *func_name = aux->value;

  symbol *func;
  // check if table is already declared
  current = get_table(func_name);
  if(current == NULL) {
    current = create_table(func_name);
    // insert function in global table
    func = insert_element(tables, func_name, func_type, NULL);
    // add functions param types to global
    check_param_list(aux->sibling, func, 0);
  } else {
    func = get_element(tables, func_name);
  }
  // function is defined, we need to print
  current->print = 1;

  // add return statement to table
  insert_element(current, "return", func_type, NULL);

  // add param to table function
  check_param_list(aux->sibling, func, 1);

  check_program(aux->sibling->sibling);
}

void check_param_list(node_t *param_list, symbol *func, int is_func_def) {
  node_t *param_list_aux = param_list->child;

  // get types of params
  while(param_list_aux != NULL) {
    char *param_type = param_list_aux->child->type;

    // go to function id
    node_t *param_declaration = param_list_aux->child->sibling;
    if(param_declaration != NULL && strcmp("Id", param_declaration->type) != 0) {
      param_declaration = param_declaration->sibling;
    }

    // check if it's a function defenition
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
}


void check_call(node_t *operator_) {

}


void check_terminal(node_t *terminal) {
  if (strcmp(terminal->type, "Id") == 0) {
    symbol id = *get_element(current, terminal->value);
    terminal->type_e = id.type;
  }
  else if (strcmp(terminal->type, "IntLit") == 0) {
    terminal->type_e = strdup("int");
  }
  else if (strcmp(terminal->type, "ChrLit") == 0) {
    terminal->type_e = strdup("int");
  }
  else if (strcmp(terminal->type, "RealLit") == 0) {
    // TODO
  }
}
