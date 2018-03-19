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

%right NOT
%left MUL DIV MOD
%left PLUS MINUS
%left LE GE LT GT
%left EQ NE
%left BITWISEAND
%left BITWISEXOR
%left BITWISEOR
%left AND
%left OR
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
*/

Program: FunctionsAndDeclarations FunctionsAndDeclarationsEmpty                           {;}
       ;

FunctionsAndDeclarations: FunctionDeclaration                                             {;}
                        | Declaration                                                     {;}
                        ;

FunctionsAndDeclarationsEmpty: FunctionsAndDeclarations FunctionsAndDeclarationsEmpty     {;} 
                             | /*empty*/                                                  {;}
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
           | error SEMI                                     {;}
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

/*[Expr{COMMA Expr}]*/
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
    | ID LPAR error RPAR          {;}
    | LPAR error RPAR             {;}
    ;

CommaExpr: CommaExpr COMMA CommaExpr {;}
         | Expr                      {;}
         ;

ExpressionList: CommaExpr         {;}
              | /*empty*/         {;}
              ;
