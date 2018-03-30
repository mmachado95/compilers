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
%token RESERVED RBRACE RPAR SEMI CHRLIT_INV CHRLIT_UNT

%token <value> ID INTLIT CHRLIT REALLIT


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
            Expr ParameterList ParameterDeclaration CommaExpr ExpressionList


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
Expr: Expr ASSIGN Expr            {$$ = insert_node("Store", NULL, 2, $1, $3);}
    | Expr PLUS Expr              {$$ = insert_node("Add", NULL, 2, $1, $3);}
    | Expr MINUS Expr             {$$ = insert_node("Sub", NULL, 2, $1, $3);}
    | Expr MUL Expr               {$$ = insert_node("Mul", NULL, 2, $1, $3);}
    | Expr DIV Expr               {$$ = insert_node("Div", NULL, 2, $1, $3);}
    | Expr MOD Expr               {$$ = insert_node("Mod", NULL, 2, $1, $3);}
    | Expr OR Expr                {$$ = insert_node("Ord", NULL, 2, $1, $3);}
    | Expr AND Expr               {$$ = insert_node("And", NULL, 2, $1, $3);}
    | Expr BITWISEAND Expr        {$$ = insert_node("BitWiseAnd", NULL, 2, $1, $3);}
    | Expr BITWISEOR Expr         {$$ = insert_node("BitWiseOr", NULL, 2, $1, $3);}
    | Expr BITWISEXOR Expr        {$$ = insert_node("BitWiseXor", NULL, 2, $1, $3);}
    | Expr EQ Expr                {$$ = insert_node("Eq", NULL, 2, $1, $3);}
    | Expr NE Expr                {$$ = insert_node("Ne", NULL, 2, $1, $3);}
    | Expr LE Expr                {$$ = insert_node("Le", NULL, 2, $1, $3);}
    | Expr GE Expr                {$$ = insert_node("Ge", NULL, 2, $1, $3);}
    | Expr LT Expr                {$$ = insert_node("Lt", NULL, 2, $1, $3);}
    | Expr GT Expr                {$$ = insert_node("Gt", NULL, 2, $1, $3);}
    | PLUS Expr                   {$$ = insert_node("Plus", NULL, 1, $2);}
    | MINUS Expr                  {$$ = insert_node("Minus", NULL, 1, $2);}
    | NOT Expr                    {$$ = insert_node("Not", NULL, 1, $2);}
    | ID LPAR ExpressionList RPAR {node_t *aux = insert_node("Id", $1, 0); $$ = insert_node("Call", NULL, 1, aux, $3);}
    | ID                          {$$ = insert_node("Id", $1, 0);}
    | INTLIT                      {$$ = insert_node("INTLIT", $1, 0);}
    | CHRLIT                      {$$ = insert_node("CHRLIT", $1, 0);}
    | REALLIT                     {$$ = insert_node("REALLIT", $1, 0);}
    | LPAR CommaExpr RPAR         {$$ = $2;}
    | ID LPAR error RPAR          {$$ = insert_node("Error", NULL, 0);}
    | LPAR error RPAR             {$$ = insert_node("Error", NULL, 0);}
    ;

/*Not sure, need to test this*/
CommaExpr: CommaExpr COMMA CommaExpr {add_sibling($1, $3); $$=$1;}
         | Expr                      {$$=$1;}
         ;

ExpressionList: CommaExpr         {$$=$1;}
              | /*empty*/         {$$ = insert_node("Null", NULL, 0);}
              ;
