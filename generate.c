#include "generate.h"


char *get_llvm_type(char *type_name) {
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
    printf("%%%s = alloca %s\n", ast->child->sibling->value, get_llvm_type(ast->child->type)); //%arr = alloca i32
  }
}

void print_param_types(param_type *params) {
  param_type *aux = params;

  while (aux != NULL) {
    if (aux->next != NULL) {
      printf("%s, ", get_llvm_type(aux->name));
    } else {
      printf("%s", get_llvm_type(aux->name));
    }

    aux = aux->next;
  }
}

void print_param_types_and_ids(node_t *params) {
  node_t *aux = params;

  while (aux != NULL) {
    char *type = aux->child->type;
    char *id = aux->child->sibling->value;

    if (aux->sibling != NULL) {
      printf("%s %s, ", get_llvm_type(type), id);
    } else {
      printf("%s %s", get_llvm_type(type), id);
    }

    aux = aux->sibling;
  }
}

void generate_code_func_declaration(node_t *func_declaration) {
  char *type = func_declaration->child->type;
  char *id = func_declaration->child->sibling->value;

  // get function symbol so that we can pass the params types to function
  symbol *func_symbol = get_element(tables, id);

  printf("declare %s @%s(", get_llvm_type(type), id);
  print_param_types(func_symbol->param);
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
