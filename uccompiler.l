%{
#include <string.h>
#include <stdio.h>
int line = 1;
int col = 1;
int beginning_line;
int beginning_col;
%}

%X COMMENT COMMENT2

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
ANYCHAR [^'\n\\]
ESCAPE \\(n|t|\\|\'|\"|([0-7]{1,3}))

ID ({LETTER}|_)({LETTER}|{NUMBER}|_)*
INTLIT {NUMBER}+
CHRLIT \'{ANYCHAR}\'|\'{ESCAPE}\'
REALLIT {NUMBER}+\.{NUMBER}*|\.{NUMBER}((E|e)(\+|\-)*{NUMBER})*

%%
{CHAR} {printf("CHAR\n"); col += yyleng;}
{ELSE} {printf("ELSE\n"); col += yyleng;}
{WHILE} {printf("WHILE\n"); col += yyleng;}
{IF} {printf("IF\n"); col += yyleng;}
{INT} {printf("INT\n"); col += yyleng;}
{SHORT} {printf("SHORT\n"); col += yyleng;}
{DOUBLE} {printf("DOUBLE\n"); col += yyleng;}
{RETURN} {printf("RETURN\n"); col += yyleng;}
{VOID} {printf("VOID\n"); col += yyleng;}
{BITWISEAND} {printf("BITWISEAND\n"); col += yyleng;}
{BITWISEOR} {printf("BITWISEOR\n"); col += yyleng;}
{BITWISEXOR} {printf("BITWISEXOR\n"); col += yyleng;}
{AND} {printf("AND\n"); col += yyleng;}
{ASSIGN} {printf("ASSIGN\n"); col += yyleng;}
{MUL} {printf("MUL\n"); col += yyleng;}
{COMMA} {printf("COMMA\n"); col += yyleng;}
{DIV} {printf("DIV\n"); col += yyleng;}
{EQ} {printf("EQ\n"); col += yyleng;}
{GE} {printf("GE\n"); col += yyleng;}
{GT} {printf("GT\n"); col += yyleng;}
{LBRACE} {printf("LBRACE\n"); col += yyleng;}
{LE} {printf("LE\n"); col += yyleng;}
{LPAR} {printf("LPAR\n"); col += yyleng;}
{LT} {printf("LT\n"); col += yyleng;}
{MINUS} {printf("MINUS\n"); col += yyleng;}
{MOD} {printf("MOD\n"); col += yyleng;}
{NE} {printf("NE\n"); col += yyleng;}
{NOT} {printf("NOT\n"); col += yyleng;}
{OR} {printf("OR\n"); col += yyleng;}
{PLUS} {printf("PLUS\n"); col += yyleng;}
{RBRACE} {printf("RBRACE\n"); col += yyleng;}
{RPAR} {printf("RPAR\n"); col += yyleng;}
{SEMI} {printf("SEMI\n"); col += yyleng;}
{RESERVED} {printf("RESERVED\n"); col += yyleng;}
{INTLIT} {printf("INTLIT(%s)\n", yytext); col += yyleng;}
{ID} {printf("ID(%s)\n", yytext); col += yyleng;}
{CHRLIT} {printf("CHRLIT(%s)\n", yytext); col += yyleng;}
{REALLIT} {printf("REALLIT(%s)\n", yytext); col += yyleng;}

\n {line++; col=1;}
\r {;}
[ \t] {col++;}
. {printf("Line %d, col %d: illegal character (%s)\n", line, col - (int)yyleng, yytext);}

"/*" {BEGIN COMMENT; beginning_col = col; beginning_line = line; col += yyleng;}
<COMMENT><<EOF>> {printf("Line <%d>, col <%d>: unterminated comment\n", beginning_line, beginning_col);}
<COMMENT>"*/" {BEGIN 0;}
<COMMENT>. {col++;}
<COMMENT>\n {line++; col=1;}

"//" {BEGIN COMMENT2;}
<COMMENT2>"\n" {BEGIN 0; line++;}
<COMMENT2>. {col++;}

%%
int main() {
  yylex();
  return 0;
}
int yywrap() {
  return 1;
}
