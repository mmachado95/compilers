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

FunctionsAndDeclarations: FunctionDefinition                                              {;}
                        | FunctionDeclaration                                             {;}
                        | Declaration                                                     {;}
                        ;

FunctionsAndDeclarationsEmpty: FunctionsAndDeclarations FunctionsAndDeclarationsEmpty     {;}
                             | /*empty*/                                                  {;}
                             ;

FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody  {;}
                  ;

FunctionBody: LBRACE DeclarationsAndStatements RBRACE         {;}
            | LBRACE RBRACE                                   {;}
            ;

DeclarationsAndStatements: Statement DeclarationsAndStatements                {;}
                         | Declaration DeclarationsAndStatements              {;}
                         | Statement                                          {;}
                         | Declaration                                        {;}
                         ;

StatementWithError: Statement             {;}
                  | error SEMI            {;}
                  ;

Statement: CommaExpr SEMI                                               {;}
         | SEMI                                                         {;}
         | LBRACE StatementList RBRACE                                  {;}
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
