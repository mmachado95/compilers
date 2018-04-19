#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    //check_declaration(ast);
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
