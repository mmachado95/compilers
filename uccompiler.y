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
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MUL DIV MOD
%left BITWISEOR
%left BITWISEXOR
%left BITWISEAND
%right NOT
%nonassoc THEN
%nonassoc ELSE


%type <node> Program FunctionsAndDeclarations FunctionsAndDeclarationsEmpty FunctionDefinition
            FunctionDeclaration Declaration FunctionDeclarator TypeSpec FunctionBody Declarator
            Expr ParameterList ParameterDeclaration CommaExpr CommaExprTwo DeclarationsAndStatements
            Statement CommaParamDeclaration CommaDeclarator StatementWithError StatementList


%%
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

FunctionBody: LBRACE RBRACE                                                               {$$ = insert_node("FuncBody", NULL, 0);}
            | LBRACE DeclarationsAndStatements RBRACE                                     {
                                                                                            if($2 == NULL)
                                                                                              $$ = insert_node("FuncBody", NULL, 0);
                                                                                            else
                                                                                              $$ = insert_node("FuncBody", NULL, 1, $2);
                                                                                          }
            ;

DeclarationsAndStatements: DeclarationsAndStatements Statement                            {
                                                                                            if($1 != NULL && $2 != NULL) {
                                                                                              $$ = add_sibling($1, $2);
                                                                                            }
                                                                                            else if($1 != NULL && $2 == NULL) {
                                                                                              $$ = $1;
                                                                                            }
                                                                                            else if($1 == NULL && $2 != NULL) {
                                                                                              $$ = $2;
                                                                                            }
                                                                                            else {
                                                                                              $$ = NULL;
                                                                                            }
                                                                                          }
                         | DeclarationsAndStatements Declaration                          {
                                                                                             if($1 != NULL && $2 != NULL) {
                                                                                               $$ = add_sibling($1, $2);
                                                                                             }
                                                                                             else if($1 != NULL && $2 == NULL) {
                                                                                               $$ = $1;
                                                                                             }
                                                                                             else if($1 == NULL && $2 != NULL) {
                                                                                               $$ = $2;
                                                                                             }
                                                                                             else {
                                                                                               $$ = NULL;
                                                                                             }
                                                                                           }
                         | Statement                                                      {
                                                                                             if($1 == NULL) {
                                                                                               $$ = NULL;
                                                                                             }
                                                                                             else {
                                                                                               $$ = $1;
                                                                                             }
                                                                                          }
                         | Declaration                                                    {
                                                                                            if($1 == NULL) {
                                                                                              $$ = NULL;
                                                                                            }
                                                                                            else {
                                                                                              $$ = $1;
                                                                                            }
                                                                                          }
                         ;

StatementWithError: Statement                                                             {$$=$1;}
                  | error SEMI                                                            {$$ = insert_node("Null", NULL, 0);}
                  ;

Statement: CommaExpr SEMI                                               {$$=$1;}
         | SEMI                                                         {$$ = NULL;}
         | LBRACE StatementList RBRACE                                  { if($2 == NULL) {
                                                                            $$ = NULL;
                                                                          }
                                                                          else if($2 != NULL && $2->sibling != NULL) {
                                                                            $$ = insert_node("StatList", NULL, 1, $2);
                                                                          }
                                                                          else {
                                                                            $$ = $2;
                                                                          }
                                                                        }
         | LBRACE RBRACE                                                {$$ = NULL;}
         | LBRACE error RBRACE                                          {$$ = insert_node("Null", NULL, 0);}
         | IF LPAR CommaExpr RPAR Statement %prec THEN                  { $3 = make_node_correct($3); $5 = make_node_correct($5);
                                                                          $$ = insert_node("If", NULL, 3, $3, $5, insert_node("Null", NULL, 0));
                                                                        }
         | IF LPAR CommaExpr RPAR Statement ELSE Statement              { $3 = make_node_correct($3); $5 = make_node_correct($5); $7 = make_node_correct($7);
                                                                          $$ = insert_node("If", NULL, 3, $3, $5, $7);
                                                                        }
         | WHILE LPAR CommaExpr RPAR Statement                          { $3 = make_node_correct($3); $5 = make_node_correct($5);
                                                                          $$ = insert_node("While", NULL, 2, $3, $5);}
         | RETURN CommaExpr SEMI                                        { $2 = make_node_correct($2);
                                                                          $$ = insert_node("Return", NULL, 1, $2);}
         | RETURN SEMI                                                  {$$ = insert_node("Return", NULL, 1, insert_node("Null", NULL, 0));}
         ;

StatementList: StatementList StatementWithError                          {
                                                                            if($1 == NULL && $2 == NULL) {
                                                                              $$=NULL;
                                                                            }
                                                                            else if($1 != NULL && $2 == NULL) {
                                                                              $$=$1;
                                                                            }
                                                                            else if($1 == NULL && $2 != NULL) {
                                                                              $$=$2;
                                                                            }
                                                                            else {
                                                                              $$ = add_sibling($1, $2);
                                                                            }
                                                                          }
             | StatementWithError                                        {$$=$1;}
             ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI       {$$ = insert_node("FuncDeclaration", NULL, 2, $1, $2);}
                   ;

FunctionDeclarator: ID LPAR ParameterList RPAR              {$$ = add_sibling(insert_node("Id", $1, 0), $3);}
                  ;

ParameterList: ParameterDeclaration CommaParamDeclaration   {$$ = insert_node("ParamList", NULL, 2, $1, $2);}
             ;

ParameterDeclaration: TypeSpec ID                           {$$ = insert_node("ParamDeclaration", NULL, 2, $1, insert_node("Id", $2, 0));}
                    | TypeSpec                              {$$ = insert_node("ParamDeclaration", NULL, 1, $1);}
                    ;

CommaParamDeclaration: COMMA ParameterDeclaration CommaParamDeclaration           {$$ = add_sibling($2, $3);}
                     | /*empty*/                                                  {$$ = NULL;}
                     ;

Declaration: TypeSpec Declarator CommaDeclarator SEMI       {
                                                              $2 = add_sibling($2, $3);
                                                              insert_node_special($1, $2);
                                                              $$ = $2;
                                                            }
           | error SEMI                                     {$$ = insert_node("Null", NULL, 0);}
           ;

CommaDeclarator: COMMA Declarator CommaDeclarator           {$$ = add_sibling($2, $3);}
               | /*empty*/                                  {$$ = NULL;}
               ;

TypeSpec: CHAR    {$$ = insert_node("Char", NULL, 0);}
        | INT     {$$ = insert_node("Int", NULL, 0);}
        | VOID    {$$ = insert_node("Void", NULL, 0);}
        | SHORT   {$$ = insert_node("Short", NULL, 0);}
        | DOUBLE  {$$ = insert_node("Double", NULL, 0);}
        ;

Declarator: ID ASSIGN Expr        {$$ = insert_node("Declaration", NULL, 2, insert_node("Id", $1, 0), $3);}
          | ID                    {$$ = insert_node("Declaration", NULL, 1, insert_node("Id", $1, 0));}
          ;

Expr: Expr ASSIGN Expr            {$$ = insert_node("Store", NULL, 2, $1, $3);}
    | Expr PLUS Expr              {$$ = insert_node("Add", NULL, 2, $1, $3);}
    | Expr MINUS Expr             {$$ = insert_node("Sub", NULL, 2, $1, $3);}
    | Expr MUL Expr               {$$ = insert_node("Mul", NULL, 2, $1, $3);}
    | Expr DIV Expr               {$$ = insert_node("Div", NULL, 2, $1, $3);}
    | Expr MOD Expr               {$$ = insert_node("Mod", NULL, 2, $1, $3);}
    | Expr OR Expr                {$$ = insert_node("Or", NULL, 2, $1, $3);}
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
    | PLUS Expr %prec NOT         {$$ = insert_node("Plus", NULL, 1, $2);}
    | MINUS Expr %prec NOT        {$$ = insert_node("Minus", NULL, 1, $2);}
    | NOT Expr                    {$$ = insert_node("Not", NULL, 1, $2);}
    | ID LPAR CommaExprTwo RPAR   {$$ = insert_node("Call", NULL, 2, insert_node("Id", $1, 0), $3);}
    | ID LPAR RPAR                {$$ = insert_node("Call", NULL, 1, insert_node("Id", $1, 0));}
    | ID                          {$$ = insert_node("Id", $1, 0);}
    | INTLIT                      {$$ = insert_node("IntLit", $1, 0);}
    | CHRLIT                      {$$ = insert_node("ChrLit", $1, 0);}
    | REALLIT                     {$$ = insert_node("RealLit", $1, 0);}
    | LPAR CommaExpr RPAR         {$$ = $2;}
    | ID LPAR error RPAR          {$$ = insert_node("Null", NULL, 0);}
    | LPAR error RPAR             {$$ = insert_node("Null", NULL, 0);}
    ;

CommaExpr: CommaExpr COMMA CommaExpr   {$$ = insert_node("Comma", NULL, 2, $1, $3);}
         | Expr                        {$$=$1;}
         ;

CommaExprTwo: CommaExprTwo COMMA CommaExprTwo   {$$ = add_sibling($1, $3);}
         | Expr                                 {$$=$1;}
         ;
