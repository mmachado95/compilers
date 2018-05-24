#include "generate.h"


char *get_type(char *type_name) {
  if (strcmp(type_name, "Int") == 0)  {
    return "i32";
  }
  else if (strcmp(type_name, "Void") == 0) {
    return "void";
  }
  return "";
}


void generate_code_program(node_t *ast) {
  //printf("\n");
}

void generate_code_declaration(node_t *ast) {
  symbol *global_var = get_element(tables, ast->child->sibling->value);
  if (global_var != NULL) {
    // TODO -> check this
    printf("@%s = global %%%s\n", ast->child->sibling->value, ast->child->type ); //@a = global %Int
  }
  else {
    // TODO -> check this
    printf("%%%s = alloca %s\n", ast->child->sibling->value, get_type(ast->child->type)); //%arr = alloca i32
  }
}


void print_param_types(node_t *params) {
  int index = 0;
  while (params != NULL) {
    if (index == 0) {
      printf("%s %s", get_type(params->child->type), params->child->sibling->value);
      index++;
    }
    else
      printf(", %s %s", get_type(params->child->type), params->child->sibling->value);
    params = params->sibling;
  }
}

void generate_code_func_declaration(node_t *ast) {
  printf("declare %s @%s(", get_type(ast->child->type), ast->child->sibling->value);
  print_param_types(ast->child->sibling->sibling->child);
  printf(")\n");
}

void generate_code_func_definition(node_t *ast) {
  // TODO -> falta fazer tudo
  generate_code(ast->child->sibling->sibling->sibling);
}


void generate_code(node_t *ast) {
  if (ast == NULL) {
    return;
  }

  //printf("TYPE: %s\n", ast->type );

  if (strcmp(ast->type, "Program") == 0) {
    generate_code_program(ast);
    generate_code(ast->child);
  }
  else if (strcmp(ast->type, "Declaration") == 0) {
    generate_code_declaration(ast);
  }
  else if (strcmp(ast->type, "FuncDeclaration") == 0) {
    generate_code_func_declaration(ast);
  }
  else if (strcmp(ast->type, "FuncDefinition") == 0) {
    generate_code_func_definition(ast);
  }
  else if (strcmp(ast->type, "FuncBody") == 0) {
    generate_code(ast->child);
  }
  else if (strcmp(ast->type, "Comma") == 0) {
    //check_comma(ast);
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
      || strcmp(ast->type, "Div") == 0) {
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
      || strcmp(ast->type, "Mod") == 0)  {
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
  else {
    generate_code(ast->child);
  }

  generate_code(ast->sibling);
}
