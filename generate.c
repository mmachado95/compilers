#include "generate.h"

int reg_count = 0;

int poww(int b, int e){
  if(e < 0) {
    return -1;
  }
  int res = 1;
  while (e) {
    if (e & 1) {
      res *= b;
    }
    e >>= 1;
    b *= b;
  }
  return res;
}

int escape_to_decimal(char *escape) {
  // get octal value without '' and without '\'
  char string_octal[3];
  int index = 0;
  for (int i = 2; i < strlen(escape) - 1; i++) {
    string_octal[index] = escape[i];
    index++;
  }
  char * pEnd = NULL;

  long octal = strtol(string_octal, &pEnd, 8);
  return octal;
  /*   int octal = atoi(string_octal);


    // convert octal to decimal
    int decimal = 0;
    int i = 0;
    int rem;
    while (octal != 0){
      rem = octal % 10;
      octal /= 10;
      decimal += rem * poww(8, i);
      ++i;
    }
    return decimal; */
}

int escape_to_number(char *escape) {
  char odd_characters[6][2] = {"n", "t", "r", "'", {'"'}, "\\"};
  int odd_charactess_values[6] = {10, 9, 13, 39, 34, 92};
  for (int i = 0; i < 6; i++) {
    if (escape[2] == odd_characters[i][0]) {
      return odd_charactess_values[i];
    }
  }
  return escape_to_decimal(escape);
}

char *get_llvm_type(char *type_name) {
  if (strcmp(type_name, "Int") == 0 || strcmp(type_name, "int") == 0)  {
    return "i32";
  }
  else if (strcmp(type_name, "Short") == 0 || strcmp(type_name, "short") == 0) {
    return "i16";
  }
  else if (strcmp(type_name, "Double") == 0 || strcmp(type_name, "double") == 0) {
    return "double";
  }
  else if (strcmp(type_name, "Char") == 0 || strcmp(type_name, "char") == 0) {
    return "i8";
  }
  else if (strcmp(type_name, "Void") == 0 || strcmp(type_name, "void") == 0) {
    return "void";
  }
  return "";
}


void generate_code_program(node_t *ast) {
  printf("declare i32 @getchar()\n");
  printf("declare i32 @putchar(i32)\n");
}

void generate_code_declaration(node_t *ast) {
  node_t *aux = ast->child;
  symbol *global_var = get_element(tables, aux->sibling->value);

  // check if symbol is global or not
  if (global_var != NULL) {
    aux = aux->sibling;

    if (aux->sibling != NULL && aux->sibling->value != NULL) {
      char *llvm_type = get_llvm_type(global_var->type);

      // if it's char print ascii value
      if(strcmp("i8", llvm_type) == 0) {
        printf("@%s = global %s %d\n", global_var->name, llvm_type, (int)aux->sibling->value[1]);
      } else {
        printf("@%s = global %s %s\n", global_var->name, llvm_type,  aux->sibling->value);
      }
    }
    else {
      printf("@%s = common global %s 0\n", global_var->name, get_llvm_type(global_var->type));
    }
  }

  else {
    printf("%%%s = alloca %s\n", aux->sibling->value, get_llvm_type(aux->type));
    if (aux->sibling->sibling != NULL) {
      node_t *aux2 = aux->sibling->sibling;
      node_t *prev = aux->sibling->sibling;

      int minus = 0;
      while (aux2 != NULL) {
        if (strcmp(aux2->type, "Minus") == 0) {
          minus = !minus;
        }
        prev = aux2;
        aux2 = aux2->child;
      }

      if (minus) {
        if (strcmp(prev->type, "ChrLit") == 0) {
          printf("store %s -%d, %s* %%%s\n", get_llvm_type(aux->type), (int) prev->value[1], get_llvm_type(aux->type), aux->sibling->value );
        }

        else if (strcmp(prev->type, "Id") == 0) {
          reg_count++;
          if (get_element(tables, prev->value) != NULL) {
            printf("%%%d = load %s, %s* @%s\n",  reg_count, get_llvm_type(aux->type), get_llvm_type(aux->type), prev->value);
          }
          else {
            printf("%%%d = load %s, %s* %%%s\n", reg_count, get_llvm_type(aux->type), get_llvm_type(aux->type), prev->value);
          }
          reg_count++;
          printf("%%%d = sub nsw i32 0, %%%d\n", reg_count, reg_count - 1);
          printf("store %s %%%d, %s* %%%s\n", get_llvm_type(aux->type), reg_count, get_llvm_type(aux->type), aux->sibling->value );
        }
        else {
          printf("store %s -%s, %s* %%%s\n", get_llvm_type(aux->type), prev->value, get_llvm_type(aux->type), aux->sibling->value );
        }
      }

      else {
        if (strcmp(prev->type, "ChrLit") == 0) {
          printf("store %s %d, %s* %%%s\n", get_llvm_type(aux->type), (int) prev->value[1], get_llvm_type(aux->type), aux->sibling->value );
        }

        else if (strcmp(prev->type, "Id") == 0) {
          reg_count++;
          if (get_element(tables, prev->value) != NULL) {// TODO -> check if it is both global and local
            printf("%%%d = load %s, %s* @%s\n", reg_count, get_llvm_type(aux->type), get_llvm_type(aux->type), prev->value);
          }
          else {
            printf("%%%d = load %s, %s* %%%s\n", reg_count, get_llvm_type(aux->type), get_llvm_type(aux->type), prev->value);
          }
          printf("store %s %%%d, %s* %%%s\n", get_llvm_type(aux->type), reg_count, get_llvm_type(aux->type), aux->sibling->value );
        }

        else {
          printf("store %s %s, %s* %%%s\n", get_llvm_type(aux->type), prev->value, get_llvm_type(aux->type), aux->sibling->value );
        }
      }
    }
  }
}

void generate_code_assign_operator(node_t *ast) {
  char end;
  if (get_element(tables, ast->child->value) != NULL) {
    end = '@';
  }
  else {
    end = '%';
  }

  node_t *aux = ast->child->sibling;
  node_t *prev = ast->child->sibling;
  int minus = 0;
  while(aux != NULL) {
    if (strcmp(aux->type, "Minus") == 0) {
      minus = !minus;
    }
    prev = aux;
    aux = aux->child;
  }

  if (minus) {
    if (strcmp(prev->type, "ChrLit") == 0) {
      printf("store %s -%d, %s* %c%s\n", get_llvm_type(ast->child->type_e), (int) prev->value[1], get_llvm_type(ast->child->type_e), end, ast->child->value );
    }

    else if (strcmp(prev->type, "Id") == 0) {
      reg_count++;
      if (get_element(tables, prev->value) != NULL) {// TODO -> check if it is both global and local
        printf("%%%d = load %s, %s* @%s\n",  reg_count,  get_llvm_type(ast->child->type_e),  get_llvm_type(ast->child->type_e), prev->value);
      }
      else {
        printf("%%%d = load %s, %s* %%%s\n", reg_count,  get_llvm_type(ast->child->type_e),  get_llvm_type(ast->child->type_e), prev->value);
      }
      reg_count++;
      printf("%%%d = sub nsw i32 0, %%%d\n", reg_count, reg_count - 1);
      printf("store %s %%%d, %s* %c%s\n", get_llvm_type(ast->child->type_e), reg_count, get_llvm_type(ast->child->type_e), end, ast->child->value );
    }
    else {
      printf("store %s -%s, %s* %c%s\n", get_llvm_type(ast->child->type_e), prev->value, get_llvm_type(ast->child->type_e), end, ast->child->value );
    }
  }

  else {
    if (strcmp(prev->type, "ChrLit") == 0) {
      printf("store %s %d, %s* %c%s\n", get_llvm_type(ast->child->type_e), (int) prev->value[1], get_llvm_type(ast->child->type_e), end, ast->child->value );
    }

    else if (strcmp(prev->type, "Id") == 0) {
      reg_count++;

      if (get_element(tables, prev->value) != NULL) {// TODO -> check if it is both global and local
        printf("%%%d = load %s, %s* @%s\n",  reg_count,  get_llvm_type(ast->child->type_e),  get_llvm_type(ast->child->type_e), prev->value);
      }
      else {
        printf("%%%d = load %s, %s* %%%s\n",  reg_count,  get_llvm_type(ast->child->type_e),  get_llvm_type(ast->child->type_e), prev->value);
      }
      printf("store %s %%%d, %s* %c%s\n", get_llvm_type(ast->child->type_e), reg_count, get_llvm_type(ast->child->type_e), end, ast->child->value );
    }

    else {
      printf("store %s %s, %s* %c%s\n", get_llvm_type(ast->child->type_e), prev->value, get_llvm_type(ast->child->type_e), end, ast->child->value );
    }
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

void print_param_types_and_ids(node_t *param_list) {
  node_t *aux = param_list->child;

  while (aux != NULL && strcmp(aux->child->type, "Void") != 0 && strcmp(aux->child->type, "void") != 0 ) {
    char *type = aux->child->type;

    if (aux->sibling != NULL) {
      printf("%s", get_llvm_type(type));
    } else {
      printf("%s", get_llvm_type(type));
    }
    if (aux->child->sibling != NULL) {
      char *id = aux->child->sibling->value;

      if (aux->sibling != NULL) {
        printf(" %%%s, ", id);
      } else {
        printf(" %%%s", id);
      }
    }

    aux = aux->sibling;
  }
}

void print_function_llvm_types(char *function_name) {
  table *global_table = get_table("Global");
  symbol *symbol = get_element(global_table, function_name);
  param_type *aux = symbol->param;

  while(aux != NULL) {
    if(aux->next != NULL) {
      printf("%s ", get_llvm_type(aux->name));
    } else {
      printf("%s", get_llvm_type(aux->name));
    }

    aux = aux->next;
  }
}

void declare_param_declaration(node_t *param_list) {
  node_t *aux = param_list->child;

  while (aux != NULL && strcmp(aux->child->type, "Void") != 0 && strcmp(aux->child->type, "void") != 0) {
    if (aux->child->sibling != NULL)
      generate_code_declaration(aux);
    aux = aux->sibling;
  }
}

void generate_code_func_declaration(node_t *func_declaration) {
  node_t *aux = func_declaration;

  char *type = aux->child->type;
  char *id = aux->child->sibling->value;

  // get function symbol so that we can pass the params types to function
  symbol *func_symbol = get_element(tables, id);
  printf("declare %s @%s(", get_llvm_type(type), id);
  print_param_types(func_symbol->param);
  declare_param_declaration(ast->child->sibling->sibling);
  printf(")\n");
}

void generate_code_func_definition(node_t *ast) {
  node_t *aux = ast->child;

  printf("define %s @%s(", get_llvm_type(aux->type), aux->sibling->value);
  print_param_types_and_ids(ast->child->sibling->sibling);
  printf(") {\n");
  declare_param_declaration(ast->child->sibling->sibling);
  generate_code(aux->sibling->sibling->sibling);
  printf("}\n");
}

void generate_code_return(node_t *ast) {
  char *llvm_type = get_llvm_type(ast->child->type_e);
  if (strcmp("Id", ast->child->type) == 0) {
    reg_count++;

    printf("%%%d = load %s, %s* %%%s\n",  reg_count, llvm_type, llvm_type, ast->child->value);
    printf("ret %s %%%d\n", llvm_type, reg_count);
  }

  else {
    printf("ret %s %s\n", llvm_type, ast->child->value);
  }
}

void generate_code_arithmetic_operator(node_t *ast) {
  if (strcmp(ast->type, "Add") == 0) {
    /* code */
  }
  else if (strcmp(ast->type, "Sub") == 0) {
    /* code */
  }
  else if (strcmp(ast->type, "Mul") == 0) {
    /* code */
  }
  else if (strcmp(ast->type, "Div") == 0) {
    /* code */
  }
}

void load_param_ids(node_t *params, symbol *func, int minus) {
  node_t *aux = params;
  param_type *param = func->param;

  while(aux != NULL) {
    if (strcmp("Id", aux->type) == 0) {
      if (get_element(tables, params->value) != NULL) // TODO -> check if it is both global and local
        printf("%%%d = load %s, %s* @%s\n", reg_count, get_llvm_type(aux->type_e), get_llvm_type(aux->type_e), aux->value);

      else
        printf("%%%d = load %s, %s* %%%s\n", reg_count, get_llvm_type(aux->type_e), get_llvm_type(aux->type_e), aux->value);
      reg_count++;

      if (strcmp(get_llvm_type(param->name), get_llvm_type(aux->type_e)) != 0) {
        printf("%%%d = sext %s %%%d to %s\n", reg_count, get_llvm_type(aux->type_e), reg_count - 1, get_llvm_type(param->name));
        reg_count++;
      }

      if (minus) {
        printf("%%%d = sub nsw i32 0, %%%d\n", reg_count, reg_count - 1);
        reg_count++;
      }
    }
    aux = aux->sibling;
    func = func->next;
  }
}

void generate_code_call(node_t *ast) {
  table *global_table = get_table("Global");
  symbol *symbol = get_element(global_table, ast->child->value);

  node_t *aux = ast->child->sibling;
  node_t *prev = ast->child->sibling;
  int minus = 0;
  while(aux != NULL) {
    if (strcmp(aux->type, "Minus") == 0) {
      minus = !minus;
    }
    prev = aux;
    aux = aux->child;
  }

  reg_count++;

  if (strcmp("putchar", ast->child->value) == 0) {
    load_param_ids(prev, symbol, minus);

    printf("%%%d = call %s @%s(", reg_count, get_llvm_type(symbol->type), ast->child->value);

    if (!minus) {
      if (strcmp(prev->type, "Id") == 0) {
        printf("%s %%%d)\n", get_llvm_type(symbol->param->name), reg_count - 1);
      }
      else if (strcmp(prev->type, "ChrLit") == 0) {
        if (strlen(prev->value) > 3) {
          printf("%s %d)\n", get_llvm_type(symbol->param->name), escape_to_number(prev->value));
        }
        else {
          printf("%s %d)\n", get_llvm_type(symbol->param->name), (int) prev->value[1]);
        }
      }
      else {
        printf("%s %s)\n", get_llvm_type(symbol->param->name), prev->value);
      }
    }

    else {
      if (strcmp(prev->type, "Id") == 0) {
        printf("%s %%%d)\n", get_llvm_type(symbol->param->name), reg_count - 1);
      }
      else if (strcmp(prev->type, "ChrLit") == 0) {
        printf("%s -%d)\n", get_llvm_type(symbol->param->name), (int) prev->value[1]);
      }
      else {
        printf("%s -%s)\n", get_llvm_type(symbol->param->name), prev->value);
      }
    }
  }

  else {
    printf("%%%d = call %s @%s(", reg_count, get_llvm_type(symbol->type), ast->child->value);
    print_function_llvm_types(ast->child->value);

    node_t *aux = ast->child->sibling;
    while (aux != NULL) {
      printf(" %s", aux->value );
      aux = aux->sibling;
    }

    printf(")\n");
  }
}

void generate_code(node_t *ast) {
  if (ast == NULL) {
    return;
  }

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
    reg_count = 0;
    generate_code(ast->child);
  }
  else if (strcmp(ast->type, "Comma") == 0) {
    //check_comma(ast);
  }
  else if (strcmp(ast->type, "Return") == 0) {
    generate_code_return(ast);
  }
  else if (strcmp(ast->type, "Store") == 0) {
    generate_code_assign_operator(ast);
  }
  else if (strcmp(ast->type, "Add") == 0
      || strcmp(ast->type, "Sub") == 0
      || strcmp(ast->type, "Mul") == 0
      || strcmp(ast->type, "Div") == 0) {
    //generate_code_arithmetic_operator(ast);
  }
  else if (strcmp(ast->type, "Plus") == 0
      || strcmp(ast->type, "Minus") == 0) {
    //generate_code_unary_operator(ast);
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
    generate_code_call(ast);
  }
  else if (strcmp(ast->type, "Id") == 0
      || strcmp(ast->type, "IntLit") == 0
      || strcmp(ast->type, "ChrLit") == 0
      || strcmp(ast->type, "RealLit") == 0) {
    //generate_code_terminal(ast);
  }
  else {
    generate_code(ast->child);
  }
  generate_code(ast->sibling);
}
