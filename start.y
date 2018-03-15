%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>

  int yylex(void);
  void yyerror (const char *s);
%}

%token <id> CHAR ELSE WHILE IF INT SHORT DOUBLE RETURN VOID BITWISEAND BITWISEOR BITWISEXOR
%token <id> AND ASSIGN MUL COMMA DIV EQ GE GT LBRACE LE LPAR LT MINUS MOD NE NOT OR PLUS
%token <id> RBRACE RPAR SEMI ID INTLIT CHRLIT REALLIT CHRLIT_INV CHRLIT_UNT

%right '='
%left COMMA

%union{
  int value;
  char* id;
}

%%
/* Notes about the EBNF grammar
* [] -> means optional
* {} -> means 0 or more times
* Para fazer o epsilon posso fazer
*/
