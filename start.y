%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>

  int yylex(void);
  void yyerror (const char *s);
%}

%token <id> CHAR ELSE WHILE IF INT SHORT DOUBLE RETURN VOID BITWISEAND BITWISEOR BITWISEXOR
%token <id> AND ASSIGN MUL COMMA DIV EQ GE GT LBRACE LE LPAR LT MINUS MOD NE NOT OR PLUS
%token <id> RESERVED RBRACE RPAR SEMI ID INTLIT CHRLIT REALLIT CHRLIT_INV CHRLIT_UNT

%right ASSIGN
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

Program: FunctionsAndDeclarations                           {;}
       | /* empty */                                        {;}
       ;

FunctionsAndDeclarations: FunctionDeclaration               {;}
                        | Declaration                       {;}
                        ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI       {;}
                   ;

FunctionDeclarator: ID LPAR ParameterList RPAR              {;}
                  ;

ParameterList: ParameterDeclaration CommaParamDeclaration   {;}
             ;

ParameterDeclaration: TypeSpec ID                           {;}
                    | TypeSpec
                    ;

CommaParamDeclaration: COMMA ParameterDeclaration           {;}
                     | /*empty*/                            {;}
                     ;

Declaration: TypeSpec Declarator CommaDeclarator SEMI       {;}
            ;

CommaDeclarator: COMMA Declarator CommaDeclarator           {;}
               |                                            {;}
               ;


TypeSpec: CHAR    {;}
        | INT     {;}
        | VOID    {;}
        | SHORT   {;}
        | DOUBLE  {;}
        ;

/*TODO add ASSIGN EXPR  optional*/
Declarator: ID              {;}
          ;
