%{
#include <string.h>
#include <stdio.h>
%}

CHAR char
ELSE else
WHILE while
IF if
INT int
SHORT short
DOUBLE double
RETURN return
VOID void
BITWISEAND "&"
BITWISEOR "|"
BITWISEXOR "^"
AND "&&"
ASSIGN "="
MUL "*"
COMMA ","
DIV "/"
EQ "=="
GE ">="
GT ">"
LBRACE "{"
LE "<="
LPAR "("
LT "<"
MINUS "-"
MOD "%"
NE "!="
NOT "!"
OR "||"
PLUS "+"
RBRACE "}"
RPAR ")"
SEMI ";"
RESERVED "auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|"inline"|"int"|"long"|"register"|"restrict"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while"|"_Bool"|"_Complex"|"_Imaginary"|"["|"]"|"++"|"--"

NUMBER [0-9]
LETTER [a-zA-Z]
ANYCHAR [^\n\' ]
ESCAPE \n|\t|\\|\'|\"|\[0-7]{1-3}

ID ({LETTER}|{NUMBER}|_)+
INTLIT {NUMBER}+
CHRLIT "'"{ANYCHAR}"'"|ESCAPE
REALLIT {NUMBER}+"."{NUMBER}*|"."{NUMBER}(("E"|"e")("+"|"-")*{NUMBER})*

%%
{CHAR} printf("CHAR\n");
{ELSE} printf("ELSE\n");
{WHILE} printf("WHILE\n");
{IF} printf("IF\n");
{INT} printf("INT\n");
{SHORT} printf("SHORT\n");
{DOUBLE} printf("DOUBLE\n");
{RETURN} printf("RETURN\n");
{VOID} printf("VOID\n");
{BITWISEAND} printf("BITWISEAND\n");
{BITWISEOR} printf("BITWISEOR\n");
{BITWISEXOR} printf("BITWISEXOR\n");
{AND} printf("AND\n");
{ASSIGN} printf("ASSIGN\n");
{MUL} printf("MUL\n");
{COMMA} printf("COMMA\n");
{DIV} printf("DIV\n");
{EQ} printf("EQ\n");
{GE} printf("GE\n");
{GT} printf("GT\n");
{LBRACE} printf("LBRACE\n");
{LE} printf("LE\n");
{LPAR} printf("LPAR\n");
{LT} printf("LT\n");
{MINUS} printf("MINUS\n");
{MOD} printf("MOD\n");
{NE} printf("NE\n");
{NOT} printf("NOT\n");
{OR} printf("OR\n");
{PLUS} printf("PLUS\n");
{RBRACE} printf("RBRACE\n");
{RPAR} printf("RPAR\n");
{SEMI} printf("SEMI\n");
{RESERVED} printf("RESERVED\n");
{INTLIT} printf("INTLIT(%s)\n", yytext);
{ID} printf("ID(%s)\n", yytext);
{CHRLIT} printf("CHRLIT(%s)\n", yytext);
{REALLIT} printf("REALLIT(%s)\n", yytext);
.|\n {}
%%

int main() {
  yylex();
  return 0;
}

int yywrap() {
  return 1;
}
