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

Program: FunctionsAndDeclarations                     {;}
       | /* empty */                                  {;}
       ;
FunctionsAndDeclarations: FunctionDefinition          {;}
                        | FunctionDeclaration         {;}
                        | Declaration                 {;}
                        ;

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody  {;}
                  ;

/* DeclarationAndStatements optional*/
FunctionBody: LBRACE RBRACE                             {;}
            | LBRACE DeclarationAndStatements RBRACE    {;}
            ;

DeclarationAndStatements: Statement DeclarationAndStatements  {;}
                        | Statement                           {;}
                        | Declaration                         {;}
                        ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI {;}
                  ;

FunctionDeclarator: ID LPAR ParameterList RPAR {;}
                  ;

/* COMMA ParameterDeclaration needs to be 0 or more times*/
Parameter: COMMA ParameterDeclaration           {;}
             | /* empty */                      {;}
             ;
ParameterList: ParameterDeclaration Parameter   {;}
             ;

/*ID optional*/
ParameterDeclaration: TypeSpec    {;}
                    | TypeSpec ID {;}
                    ;

Declaration: TypeSpec Declarator COMMA Declarator SEMI {;}
           ;

TypeSpec: CHAR    {;}
        | INT     {;}
        | VOID    {;}
        | SHORT   {;}
        | DOUBLE  {;}
        ;

/*ASSIGN EXPR is optional*/
Declarator: ID              {;}
          | ID ASSIGN Expr  {;}
          ;

/*Expr is optional*/
Statement: SEMI                                       {;}
         | Expr SEMI                                  {;}
         ;
/*Statement needs to be 0 or more times*/
RepeatableStatement: Statement                        {;}
                   | /* empty */                      {;}
                   ;
Statement: LBRACE RepeatableStatement RBRACE          {;}
         ;
/* ELSE Statement is optional*/
Statement: IF LPAR Expr RPAR Statement                {;}
         | IF LPAR Expr RPAR Statement ELSE Statement {;}
         ;
Statement: WHILE LPAR Expr RPAR Statement             {;}
         ;
/*Expr is optional*/
Statement: RETURN SEMI                                {;}
         | RETURN Expr SEMI                           {;}
         ;

Expr: Expr ASSIGN Expr      {;}
    | Expr COMMA Expr       {;}
    | Expr PLUS Expr        {;}
    | Expr MINUS Expr       {;}
    | Expr MUL Expr         {;}
    | Expr DIV Expr         {;}
    | Expr MOD Expr         {;}
    ;
Expr: Expr OR Expr          {;}
    | Expr AND Expr         {;}
    | Expr BITWISEAND Expr  {;}
    | Expr BITWISEOR Expr   {;}
    | Expr BITWISEXOR Expr  {;}
    ;
Expr: Expr EQ Expr          {;}
    | Expr NE Expr          {;}
    | Expr LE Expr          {;}
    | Expr GE Expr          {;}
    | Expr LT Expr          {;}
    | Expr GT Expr          {;}
    ;
Expr: PLUS Expr             {;}
    | MINUS Expr            {;}
    | NOT Expr              {;}
    ;
CommaExprAgain: COMMA Expr CommaExprAgain  {;}
              |                            {;}
              ;
/* Expr{COMMA Expr} */
CommaExpr: Expr CommaExprAgain      {;}
         ;
/* CommaExpr optional */
Expr: ID LPAR RPAR                  {;}
    | ID LPAR CommaExpr RPAR        {;}
    ;
Expr: ID LPAR Expr RPAR             {;}
    | INTLIT LPAR Expr RPAR         {;}
    | CHRLIT LPAR Expr RPAR         {;}
    | REALLIT LPAR Expr RPAR        {;}
    ;
%%
