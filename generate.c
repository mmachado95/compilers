#include "generate.h"


void generate_code(node_t *ast) {
  //printf("OII\n\n");
  if (ast == NULL) {
    return;
  }

  if (strcmp(ast->type, "Program") == 0) {
    //check_program(ast->child);
  }
  else if (strcmp(ast->type, "Declaration") == 0) {
    //check_declaration(ast);
  }
  else if (strcmp(ast->type, "FuncDeclaration") == 0) {
    //check_func_declaration(ast);
  }
  else if (strcmp(ast->type, "FuncDefinition") == 0) {
    //check_func_definition(ast);
  }
  else if (strcmp(ast->type, "FuncBody") == 0) {
    //check_program(ast->child);
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
    //check_program(ast->child);
  }

  //check_program(ast->sibling);
}
