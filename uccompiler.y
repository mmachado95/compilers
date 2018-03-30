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

%token CHAR ELSE IF INT SHORT DOUBLE RETURN VOID BITWISEAND BITWISEOR BITWISEXOR
%token AND ASSIGN MUL COMMA DIV EQ GE GT LBRACE LE LPAR LT MINUS MOD NE NOT OR PLUS
%token RESERVED RBRACE RPAR SEMI CHRLIT_INV CHRLIT_UNT

%token <value> ID INTLIT CHRLIT REALLIT WHILE


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
            Expr ParameterList ParameterDeclaration CommaExpr ExpressionList DeclarationsAndStatements
            Statement CommaParamDeclaration CommaDeclarator StatementWithError StatementList


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

FunctionBody: LBRACE DeclarationsAndStatements RBRACE                                     {$$ = insert_node("FunctionBody", NULL, 1, $2);}
            | LBRACE RBRACE                                                               {$$ = insert_node("FunctionBody", NULL, 0);}
            ;

DeclarationsAndStatements: DeclarationsAndStatements Statement                            {$$ = add_sibling($1, $2);}
                         | DeclarationsAndStatements Declaration                          {$$ = add_sibling($1, $2);}
                         | Statement                                                      {$$=$1;}
                         | Declaration                                                    {$$=$1;}
                         ;

StatementWithError: Statement                                                             {$$=$1;}
                  | error SEMI                                                            {printf("\n\nNOT IMPLEMENTED YET 2\n\n");}
                  ;

Statement: CommaExpr SEMI                                               {$$=$1;}
         | SEMI                                                         {printf("\n\nNOT IMPLEMENTED YET 4\n\n");}
         | LBRACE StatementList RBRACE                                  {$$=$2;}
         | LBRACE RBRACE                                                {printf("\n\nNOT IMPLEMENTED YET 6\n\n");}
         | LBRACE error RBRACE                                          {printf("\n\nNOT IMPLEMENTED YET 7\n\n");}
         | IF LPAR CommaExpr RPAR Statement %prec THEN                  {printf("\n\nNOT IMPLEMENTED YET 8\n\n");}
         | IF LPAR CommaExpr RPAR Statement ELSE Statement              {printf("\n\nNOT IMPLEMENTED YET 9\n\n");}
         | WHILE LPAR CommaExpr RPAR Statement                          {insert_node("While", NULL, 2, $3, $5);}
         | RETURN CommaExpr SEMI                                        {add_sibling(insert_node("Return", NULL, 0), $2);}
         | RETURN SEMI                                                  {insert_node("Return", NULL, 0);}
         ;

StatementList: StatementList StatementWithError                          {add_sibling(insert_node("Statlist", NULL, 0), $2);}
             | StatementWithError                                        {$$=$1;}
             ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI       {$$ = insert_node("FuncDeclaration", NULL, 2, $1, $2);}
                   ;

FunctionDeclarator: ID LPAR ParameterList RPAR              {$$ = add_sibling(insert_node("Id", $1, 0), $3);}
                  ;

ParameterList: ParameterDeclaration CommaParamDeclaration   {$$ = insert_node("ParamList", NULL, 2, $1, $2);}
             ;

ParameterDeclaration: TypeSpec ID                           {$$ = insert_node("ParamDeclaration", NULL, 2, $1, $2);}
                    | TypeSpec                              {$$ = insert_node("ParamDeclaration", NULL, 1, $1);}
                    ;

CommaParamDeclaration: COMMA ParameterList                  {printf("\n\nNOT IMPLEMENTED YET 10\n\n");}
                     | /*empty*/                            {$$ = NULL;}
                     ;

Declaration: TypeSpec Declarator CommaDeclarator SEMI       {$$ = insert_node("Declaration", NULL, 3, $1, $2);}
           | error SEMI                                     {printf("\n\nNOT IMPLEMENTED YET 12\n\n");}
           ;

CommaDeclarator: COMMA Declarator CommaDeclarator           {printf("\n\nNOT IMPLEMENTED YET 13\n\n");}
               | /*empty*/                                  {$$ = NULL;}
               ;

TypeSpec: CHAR    {$$ = insert_node("Char", NULL, 0);}
        | INT     {$$ = insert_node("Int", NULL, 0);}
        | VOID    {$$ = insert_node("Void", NULL, 0);}
        | SHORT   {$$ = insert_node("Short", NULL, 0);}
        | DOUBLE  {$$ = insert_node("Double", NULL, 0);}
        ;

/*ASSIGN EXPR  optional*/
Declarator: ID ASSIGN Expr        {$$ = add_sibling(insert_node("Id", $1, 0), $3);}
          | ID                    {$$ = insert_node("Id", $1, 0);}
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
    | ID LPAR ExpressionList RPAR {$$ = insert_node("Call", NULL, 2, insert_node("Id", $1, 0), $3);}
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
