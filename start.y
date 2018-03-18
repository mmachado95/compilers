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
%left DIV MUL MOD
%left PLUS MINUS
%left AND OR
%right BITWISEAND BITWISEOR BITWISEXOR
%left EQ NE
%left LE GE LT GT
%right NOT

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

CommaParamDeclaration: COMMA ParameterList                  {;}
                     | /*empty*/                            {;}
                     ;

Declaration: TypeSpec Declarator CommaDeclarator SEMI       {;}
            ;

CommaDeclarator: COMMA Declarator CommaDeclarator           {;}
               | /*empty*/                                  {;}
               ;

TypeSpec: CHAR    {;}
        | INT     {;}
        | VOID    {;}
        | SHORT   {;}
        | DOUBLE  {;}
        ;

/*ASSIGN EXPR  optional*/
Declarator: ID ASSIGN Expr        {;}
          | ID                    {;}
          ;

/*TODO [Expr{COMMA Expr}]*/
Expr: Expr ASSIGN Expr            {;}
    | Expr PLUS Expr              {;}
    | Expr MINUS Expr             {;}
    | Expr MUL Expr               {;}
    | Expr DIV Expr               {;}
    | Expr MOD Expr               {;}
    | Expr OR Expr                {;}
    | Expr AND Expr               {;}
    | Expr BITWISEAND Expr        {;}
    | Expr BITWISEOR Expr         {;}
    | Expr BITWISEXOR Expr        {;}
    | Expr EQ Expr                {;}
    | Expr NE Expr                {;}
    | Expr LE Expr                {;}
    | Expr GE Expr                {;}
    | Expr LT Expr                {;}
    | Expr GT Expr                {;}
    | PLUS Expr                   {;}
    | MINUS Expr                  {;}
    | NOT Expr                    {;}
    | ID LPAR ExpressionList RPAR {;}
    | ID                          {;}
    | INTLIT                      {;}
    | CHRLIT                      {;}
    | REALLIT                     {;}
    | LPAR CommaExpr RPAR         {;}
    ;

CommaExpr: CommaExpr COMMA CommaExpr {;}
         | Expr                      {;}
         ;

ExpressionList: CommaExpr         {;}
              | /*empty*/         {;}
              ;
