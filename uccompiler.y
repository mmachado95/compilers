%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include "ast.h"

  int yylex(void);
  void yyerror (const char *s);
%}

%union{
  char* value;
  struct node *node;
}

%token CHAR ELSE WHILE IF INT SHORT DOUBLE RETURN VOID BITWISEAND BITWISEOR BITWISEXOR
%token AND ASSIGN MUL COMMA DIV EQ GE GT LBRACE LE LPAR LT MINUS MOD NE NOT OR PLUS
%token RESERVED RBRACE RPAR SEMI REALLIT CHRLIT_INV CHRLIT_UNT

%token <value> ID INTLIT CHRLIT


%left COMMA
%right ASSIGN
%left OR
%left AND
%left BITWISEOR
%left BITWISEXOR
%left BITWISEAND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MUL DIV MOD
%right NOT

%nonassoc THEN
%nonassoc ELSE


%type <node> Program FunctionsAndDeclarations FunctionsAndDeclarationsEmpty FunctionDefinition
            FunctionDeclaration Declaration FunctionDeclarator TypeSpec FunctionBody Declarator
            Expr ParameterList ParameterDeclaration


%%
/* Notes about the EBNF grammar
* [] -> means optional
* {} -> means 0 or more times
*/

Program: FunctionsAndDeclarations FunctionsAndDeclarationsEmpty                           {ast = insert_node("Program", NULL, 2, $1, $2);}
       ;

FunctionsAndDeclarations: FunctionDefinition                                              {$$=$1;}
                        | FunctionDeclaration                                             {$$=$1;}
                        | Declaration                                                     {$$=$1;}
                        ;

FunctionsAndDeclarationsEmpty: FunctionsAndDeclarations FunctionsAndDeclarationsEmpty     {$$ = add_sibling($1, $2);}
                             | /*empty*/                                                  {$$ = NULL;}
                             ;

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody                              {$$ = insert_node("FuncDefinition", NULL, 3, $1, $2, $3);}
                  ;

FunctionBody: LBRACE DeclarationsAndStatements RBRACE         {;}
            | LBRACE RBRACE                                   {;}
            ;

DeclarationsAndStatements: DeclarationsAndStatements Statement                {;}
                         | DeclarationsAndStatements Declaration              {;}
                         | Statement                                          {;}
                         | Declaration                                        {;}
                         ;

StatementWithError: Statement             {;}
                  | error SEMI            {;}
                  ;

Statement: CommaExpr SEMI                                               {;}
         | SEMI                                                         {;}
         | LBRACE StatementList RBRACE                                  {;}
         | LBRACE RBRACE                                                {;}
         | LBRACE error RBRACE                                          {;}
         | IF LPAR CommaExpr RPAR Statement %prec THEN                  {;}
         | IF LPAR CommaExpr RPAR Statement ELSE Statement              {;}
         | WHILE LPAR CommaExpr RPAR Statement                          {;}
         | RETURN CommaExpr SEMI                                        {;}
         | RETURN SEMI                                                  {;}
         ;

StatementList: StatementList StatementWithError                          {;}
             | StatementWithError                                        {;}
             ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI       {$$ = insert_node("FuncDeclaration", NULL, 2, $1, $2);}
                   ;

FunctionDeclarator: ID LPAR ParameterList RPAR              {printf("---%s----\n", $1);$$ = add_sibling(insert_node("Id", $1, 0), $3);}
                  ;

ParameterList: ParameterDeclaration CommaParamDeclaration   {;}
             ;

ParameterDeclaration: TypeSpec ID                           {$$ = insert_node("ParamDeclaration", NULL, 2, $1, $2);}
                    | TypeSpec                              {$$ = insert_node("ParamDeclaration", NULL, 1, $1);}
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

TypeSpec: CHAR    {$$ = insert_node("Char", NULL, 0);}
        | INT     {$$ = insert_node("Int", NULL, 0);}
        | VOID    {$$ = insert_node("Void", NULL, 0);}
        | SHORT   {$$ = insert_node("Short", NULL, 0);}
        | DOUBLE  {$$ = insert_node("Double", NULL, 0);}
        ;

/*ASSIGN EXPR  optional*/
Declarator: ID ASSIGN Expr        {printf("ALLO\n"); $$ = insert_node("Declarator", $1, 2, $1, $3);}
          | ID                    {printf("Ola colega\n"); $$ = insert_node("Id", $1, 0);}
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
