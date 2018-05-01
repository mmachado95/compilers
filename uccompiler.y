%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include "ast.h"

  int yylex(void);
  void yyerror (const char *s);

  extern int line, col;
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


%type <node> Program FunctionsAndDeclarations FunctionDefinition
            FunctionDeclaration Declaration FunctionDeclarator TypeSpec FunctionBody Declarator
            Expr ParameterList ParameterDeclaration CommaExpr CommaExprTwo DeclarationsAndStatements
            Statement CommaParamDeclaration CommaDeclarator StatementWithError StatementList ID_terminal


%%
Program: FunctionsAndDeclarations                                                         {ast = insert_node(line, col-yylen, "Program", NULL, 1, $1);}
       ;

FunctionsAndDeclarations: FunctionsAndDeclarations FunctionDefinition                     {$$ = add_sibling($1, $2);}
                        | FunctionsAndDeclarations FunctionDeclaration                    {$$ = add_sibling($1, $2);}
                        | FunctionsAndDeclarations Declaration                            {$$ = add_sibling($1, $2);}
                        | FunctionDefinition                                              {$$ = $1;}
                        | FunctionDeclaration                                             {$$ = $1;}
                        | Declaration                                                     {$$ = $1;}
                        ;

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody                              {$$ = insert_node(line, col-yylen, "FuncDefinition", NULL, 3, $1, $2, $3);}
                  ;

FunctionBody: LBRACE RBRACE                                                               {$$ = insert_node(line, col-yylen, "FuncBody", NULL, 0);}
            | LBRACE DeclarationsAndStatements RBRACE                                     {$$ = insert_node(line, col-yylen, "FuncBody", NULL, 1, $2);}
            ;

DeclarationsAndStatements: Statement DeclarationsAndStatements                            {$$ = add_sibling($1, $2);}
                         | Declaration DeclarationsAndStatements                          {$$ = add_sibling($1, $2);}
                         | Statement                                                      {$$ = $1;}
                         | Declaration                                                    {$$ = $1;}
                         ;

StatementWithError: Statement                                                             {$$=$1;}
                  | error SEMI                                                            {$$ = NULL;}
                  ;

Statement: CommaExpr SEMI                                               {$$=$1;}
         | SEMI                                                         {$$ = NULL;}
         | LBRACE StatementList RBRACE                                  { if($2 != NULL && $2->sibling != NULL) {
                                                                            $$ = insert_node(line, col-yylen, "StatList", NULL, 1, $2);
                                                                          }
                                                                          else {
                                                                            $$ = $2;
                                                                          }
                                                                        }
         | LBRACE RBRACE                                                {$$ = NULL;}
         | LBRACE error RBRACE                                          {$$ = NULL;}
         | IF LPAR CommaExpr RPAR StatementWithError %prec THEN         { $3 = make_node_correct(line, col-yylen, $3); $5 = make_node_correct(line, col-yylen, $5);
                                                                          $$ = insert_node(line, col-yylen, "If", NULL, 3, $3, $5, insert_node(line, col-yylen, "Null", NULL, 0));
                                                                        }
         | IF LPAR CommaExpr RPAR StatementWithError ELSE StatementWithError  { $3 = make_node_correct(line, col-yylen, $3); $5 = make_node_correct(line, col-yylen, $5); $7 = make_node_correct(line, col-yylen, $7);
                                                                                $$ = insert_node(line, col-yylen, "If", NULL, 3, $3, $5, $7);
                                                                              }
         | WHILE LPAR CommaExpr RPAR StatementWithError                 { $3 = make_node_correct(line, col-yylen, $3); $5 = make_node_correct(line, col-yylen, $5);
                                                                          $$ = insert_node(line, col-yylen, "While", NULL, 2, $3, $5);}
         | RETURN CommaExpr SEMI                                        { $2 = make_node_correct(line, col-yylen, $2);
                                                                          $$ = insert_node(line, col-yylen, "Return", NULL, 1, $2);}
         | RETURN SEMI                                                  {$$ = insert_node(line, col-yylen, "Return", NULL, 1, insert_node(line, col-yylen, "Null", NULL, 0));}
         ;

StatementList: StatementList StatementWithError                          {$$ = add_sibling($1, $2);}
             | StatementWithError                                        {$$=$1;}
             ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI       {$$ = insert_node(line, col-yylen, "FuncDeclaration", NULL, 2, $1, $2);}
                   ;

FunctionDeclarator: ID_terminal LPAR ParameterList RPAR     {$$ = add_sibling($1, $3);}
                  ;

ParameterList: ParameterDeclaration CommaParamDeclaration   {$$ = insert_node(line, col-yylen, "ParamList", NULL, 2, $1, $2);}
             ;

ParameterDeclaration: TypeSpec ID_terminal                           {$$ = insert_node(line, col-yylen, "ParamDeclaration", NULL, 2, $1, $2);}
                    | TypeSpec                              {$$ = insert_node(line, col-yylen, "ParamDeclaration", NULL, 1, $1);}
                    ;

CommaParamDeclaration: COMMA ParameterDeclaration CommaParamDeclaration           {$$ = add_sibling($2, $3);}
                     | /*empty*/                                                  {$$ = NULL;}
                     ;

Declaration: TypeSpec Declarator CommaDeclarator SEMI       {
                                                              $2 = add_sibling($2, $3);
                                                              insert_node_special($1, $2);
                                                              $$ = $2;
                                                            }
           | error SEMI                                     {$$ = NULL;}
           ;

CommaDeclarator: COMMA Declarator CommaDeclarator           {$$ = add_sibling($2, $3);}
               | /*empty*/                                  {$$ = NULL;}
               ;

TypeSpec: CHAR    {$$ = insert_node(line, col - 4, "Char", NULL, 0);}
        | INT     {$$ = insert_node(line, col - 3, "Int", NULL, 0);}
        | VOID    {$$ = insert_node(line, col - 4, "Void", NULL, 0);}
        | SHORT   {$$ = insert_node(line, col - 5, "Short", NULL, 0);}
        | DOUBLE  {$$ = insert_node(line, col - 6, "Double", NULL, 0);}
        ;

Declarator: ID_terminal ASSIGN Expr         {$$ = insert_node(line, col, "Declaration", NULL, 2, $1, $3);}
          | ID_terminal                     {$$ = insert_node(line, col, "Declaration", NULL, 1, $1);}
          ;


ID_terminal: ID                             {$$ = insert_node(line, col-strlen($1), "Id", $1, 0);}


Expr: Expr ASSIGN Expr                      {$$ = insert_node(line, col, "Store", NULL, 2, $1, $3);}
    | Expr PLUS Expr                        {$$ = insert_node(line, col, "Add", NULL, 2, $1, $3);}
    | Expr MINUS Expr                       {$$ = insert_node(line, col, "Sub", NULL, 2, $1, $3);}
    | Expr MUL Expr                         {$$ = insert_node(line, col, "Mul", NULL, 2, $1, $3);}
    | Expr DIV Expr                         {$$ = insert_node(line, col, "Div", NULL, 2, $1, $3);}
    | Expr MOD Expr                         {$$ = insert_node(line, col, "Mod", NULL, 2, $1, $3);}
    | Expr OR Expr                          {$$ = insert_node(line, col, "Or", NULL, 2, $1, $3);}
    | Expr AND Expr                         {$$ = insert_node(line, col, "And", NULL, 2, $1, $3);}
    | Expr BITWISEAND Expr                  {$$ = insert_node(line, col, "BitWiseAnd", NULL, 2, $1, $3);}
    | Expr BITWISEOR Expr                   {$$ = insert_node(line, col, "BitWiseOr", NULL, 2, $1, $3);}
    | Expr BITWISEXOR Expr                  {$$ = insert_node(line, col, "BitWiseXor", NULL, 2, $1, $3);}
    | Expr EQ Expr                          {$$ = insert_node(line, col, "Eq", NULL, 2, $1, $3);}
    | Expr NE Expr                          {$$ = insert_node(line, col, "Ne", NULL, 2, $1, $3);}
    | Expr LE Expr                          {$$ = insert_node(line, col, "Le", NULL, 2, $1, $3);}
    | Expr GE Expr                          {$$ = insert_node(line, col, "Ge", NULL, 2, $1, $3);}
    | Expr LT Expr                          {$$ = insert_node(line, col, "Lt", NULL, 2, $1, $3);}
    | Expr GT Expr                          {$$ = insert_node(line, col, "Gt", NULL, 2, $1, $3);}
    | PLUS Expr %prec NOT                   {$$ = insert_node(line, col, "Plus", NULL, 1, $2);}
    | MINUS Expr %prec NOT                  {$$ = insert_node(line, col, "Minus", NULL, 1, $2);}
    | NOT Expr                              {$$ = insert_node(line, col, "Not", NULL, 1, $2);}
    | ID_terminal LPAR CommaExprTwo RPAR    {$$ = insert_node(line, col, "Call", NULL, 2, $1, $3);}
    | ID_terminal LPAR RPAR                 {$$ = insert_node(line, col, "Call", NULL, 1, $1);}
    | ID_terminal                           {$$ = $1;}
    | INTLIT                                {$$ = insert_node(line, col, "IntLit", $1, 0);}
    | CHRLIT                                {$$ = insert_node(line, col, "ChrLit", $1, 0);}
    | REALLIT                               {$$ = insert_node(line, col, "RealLit", $1, 0);}
    | LPAR CommaExpr RPAR                   {$$ = $2;}
    | ID_terminal LPAR error RPAR           {$$ = NULL;}
    | LPAR error RPAR                       {$$ = NULL;}
    ;

CommaExpr: CommaExpr COMMA CommaExpr        {$$ = insert_node(line, col-yylen, "Comma", NULL, 2, $1, $3);}
         | Expr                             {$$ = $1;}
         ;

CommaExprTwo: CommaExprTwo COMMA CommaExprTwo   {$$ = add_sibling($1, $3);}
         | Expr                                 {$$ = $1;}
         ;
