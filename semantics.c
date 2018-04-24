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
  }
  else if (strcmp(ast->type, "FuncDefinition") == 0) {
    check_func_definition(ast);
  }
  // TODO check if func body ahs child or not
  else if (strcmp(ast->type, "FuncBody") == 0) {
    check_program(ast);
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
    //check_call(ast);
  }
  else if (strcmp(ast->type, "Id") == 0
      || strcmp(ast->type, "IntLit") == 0
      || strcmp(ast->type, "ChrLit") == 0
      || strcmp(ast->type, "RealLit") == 0) {
    //check_terminal(ast);
  }

  // iterate to the next node
  check_program(ast->sibling);
}

void check_declaration(node_t *declaration) {
  node_t *aux = declaration->child;

  // if symbol is not in table, add to table
  if (get_element(tables, aux->sibling->value) == NULL) {
    insert_element(tables, aux->sibling->value, aux->type, NULL);
  } else {
    // should print an error of declaration already declared
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

    // go to the first paramdeclaration
    aux = aux->sibling->child;

    // get types of params
    while(aux != NULL) {
      insert_type(aux->child->type, func_declaration);

      // if the next node is an id, skip it
      if(aux->sibling != NULL && strcmp("Id", aux->sibling->type) == 0) {
        aux = aux->sibling;
      }

      aux = aux->sibling;
    }

  }

  current = tables;
}


void check_func_definition(node_t *func_definition) {
  // save type of function
  char *func_type = func_definition->child->type;

  // go to FunctionDeclarator
  node_t *aux = func_definition->child->sibling;

  // if table for function doesn't exist already
  if (get_table(aux->value) == NULL) {
    char *func_name = aux->value;
    // create table for function
    current = create_table(func_name);

    // insert element in global table
    symbol *func_definition = insert_element(tables, func_name, func_type, NULL);

    // go to the first paramdeclaration
    aux = aux->sibling->child;

    // get types of params
    while(aux != NULL) {
      insert_type(aux->child->type, func_definition);

      // if the next node is an id, skip it
      if(aux->sibling != NULL && strcmp("Id", aux->sibling->type) == 0) {
        aux = aux->sibling;
      }

      aux = aux->sibling;
    }

  }

  current = tables;
}
