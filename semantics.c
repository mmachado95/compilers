#include "semantics.h"

void check_program(node_t *ast) {
  // stop condition
  if (ast == NULL) {
    return;
  }

  if (strcmp(ast->type, "Program") == 0) {
    check_program(ast->child);
  }
  if (strcmp(ast->type, "Declaration") == 0) {
    check_declaration(ast);
  }
  if (strcmp(ast->type, "FuncDeclaration") == 0) {
    //check_func_declaration(ast);
  }
  if (strcmp(ast->type, "FuncDefinition") == 0) {
    //check_func_definition(ast);
  }
  if (strcmp(ast->type, "ParamList") == 0) {
    //check_param_list(ast);
  }
  // TODO check if func body ahs child or not
  if (strcmp(ast->type, "FuncBody") == 0) {
    check_program(ast);
  }
  if (strcmp(ast->type, "If") == 0) {
    //check_if(ast);
  }
  if (strcmp(ast->type, "While") == 0) {
    //check_while(ast);
  }
  if (strcmp(ast->type, "Return") == 0) {
    //check_return(ast);
  }
  if (strcmp(ast->type, "Store") == 0) {
    //check_assign_operator(ast);
  }
  if (strcmp(ast->type, "Add") == 0
      || strcmp(ast->type, "Sub") == 0
      || strcmp(ast->type, "Mul") == 0
      || strcmp(ast->type, "Div") == 0
      || strcmp(ast->type, "Mod") == 0) {
    //check_arithmetic_operator(ast);
  }
  if (strcmp(ast->type, "Plus") == 0
      || strcmp(ast->type, "Minus") == 0) {
    //check_unary_operator(ast);
  }
  if (strcmp(ast->type, "Eq") == 0
      || strcmp(ast->type, "Ne") == 0
      || strcmp(ast->type, "Le") == 0
      || strcmp(ast->type, "Ge") == 0
      || strcmp(ast->type, "Lt") == 0
      || strcmp(ast->type, "Gt") == 0
      || strcmp(ast->type, "Mod") == 0) {
    //check_relational_operator(ast);
  }
  if (strcmp(ast->type, "Not") == 0
      || strcmp(ast->type, "And") == 0
      || strcmp(ast->type, "Or") == 0) {
    //check_logical_operator(ast);
  }
  if (strcmp(ast->type, "BitWiseAnd") == 0
      || strcmp(ast->type, "BitWiseOr") == 0
      || strcmp(ast->type, "BitWiseXor") == 0) {
    //check_bitwise_operator(ast);
  }
  if (strcmp(ast->type, "Call") == 0) {
    //check_call(ast);
  }
  if (strcmp(ast->type, "Id") == 0
      || strcmp(ast->type, "IntLit") == 0
      || strcmp(ast->type, "ChrLit") == 0
      || strcmp(ast->type, "RealLit") == 0) {
    //check_terminal(ast);
  }
}

void check_declaration(node_t *declaration) {
  node_t *aux = declaration->child;

  if (get_element(current, declaration->value) != NULL) {
    return;
  }
  insert_element(current, aux->type, aux->value, NULL);
}

/*void check_func_declaration(node_t *func_declaration) {*/
  /*type *param_list_types = NULL;*/
  /*// save type of function*/
  /*char *func_type = func_declaration->child->type;*/

  /*// go to FunctionDeclarator*/
  /*node_t *aux = func_declaration->child->sibling;*/

  /*// check if table for function already exists*/
  /*if (get_table(aux->value) == NULL) {*/
    /*// create table for function*/
    /*current = create_table(aux->value);*/

    /*// go to paramdeclaration*/
    /*aux = aux->sibling->child;*/

    /*// get types of params*/
    /*param_list_types = NULL;*/
    /*get_param_list_type)bbus(aux); */
    /*printf("%d\n", (int)param_list_types);*/

    /*printf("=================\n");*/
    /*printf("%s\n", param_list_types->name);*/
    /*while(param_list_types != NULL) {*/
      /*printf("%s\n", param_list_types->name);*/
      /*param_list_types = param_list_types->next;*/
    /*}*/
    /*printf("=================\n");*/
  /*}*/

  /*free_param_list_types();*/
  /*current = tables;*/
/*}*/
