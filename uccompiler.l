%{
#include <string.h>
#include <stdio.h>
#define PRINTF if(errors == 0) printf
int line = 1;
int col = 1;
int beginning_line;
int beginning_col;
int errors;
%}

%X COMMENT
%X COMMENT2

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
EXPOENTE (E|e)(\+|\-)*{NUMBER}+

ID ({LETTER}|_)({LETTER}|{NUMBER}|_)*
INTLIT {NUMBER}+
CHRLIT \'{ANYCHAR}\'|\'{ESCAPE}\'
REALLIT ({NUMBER}+\.{NUMBER}*{EXPOENTE}*)|(\.{NUMBER}+{EXPOENTE}*)|({NUMBER}+{EXPOENTE})

%%
{CHAR} {PRINTF("CHAR\n"); col += yyleng;}
{ELSE} {PRINTF("ELSE\n"); col += yyleng;}
{WHILE} {PRINTF("WHILE\n"); col += yyleng;}
{IF} {PRINTF("IF\n"); col += yyleng;}
{INT} {PRINTF("INT\n"); col += yyleng;}
{SHORT} {PRINTF("SHORT\n"); col += yyleng;}
{DOUBLE} {PRINTF("DOUBLE\n"); col += yyleng;}
{RETURN} {PRINTF("RETURN\n"); col += yyleng;}
{VOID} {PRINTF("VOID\n"); col += yyleng;}
{BITWISEAND} {PRINTF("BITWISEAND\n"); col += yyleng;}
{BITWISEOR} {PRINTF("BITWISEOR\n"); col += yyleng;}
{BITWISEXOR} {PRINTF("BITWISEXOR\n"); col += yyleng;}
{AND} {PRINTF("AND\n"); col += yyleng;}
{ASSIGN} {PRINTF("ASSIGN\n"); col += yyleng;}
{MUL} {PRINTF("MUL\n"); col += yyleng;}
{COMMA} {PRINTF("COMMA\n"); col += yyleng;}
{DIV} {PRINTF("DIV\n"); col += yyleng;}
{EQ} {PRINTF("EQ\n"); col += yyleng;}
{GE} {PRINTF("GE\n"); col += yyleng;}
{GT} {PRINTF("GT\n"); col += yyleng;}
{LBRACE} {PRINTF("LBRACE\n"); col += yyleng;}
{LE} {PRINTF("LE\n"); col += yyleng;}
{LPAR} {PRINTF("LPAR\n"); col += yyleng;}
{LT} {PRINTF("LT\n"); col += yyleng;}
{MINUS} {PRINTF("MINUS\n"); col += yyleng;}
{MOD} {PRINTF("MOD\n"); col += yyleng;}
{NE} {PRINTF("NE\n"); col += yyleng;}
{NOT} {PRINTF("NOT\n"); col += yyleng;}
{OR} {PRINTF("OR\n"); col += yyleng;}
{PLUS} {PRINTF("PLUS\n"); col += yyleng;}
{RBRACE} {PRINTF("RBRACE\n"); col += yyleng;}
{RPAR} {PRINTF("RPAR\n"); col += yyleng;}
{SEMI} {PRINTF("SEMI\n"); col += yyleng;}
{RESERVED} {PRINTF("RESERVED\n"); col += yyleng;}
{INTLIT} {PRINTF("INTLIT(%s)\n", yytext); col += yyleng;}
{ID} {PRINTF("ID(%s)\n", yytext); col += yyleng;}
{CHRLIT} {PRINTF("CHRLIT(%s)\n", yytext); col += yyleng;}
{REALLIT} {PRINTF("REALLIT(%s)\n", yytext); col += yyleng;}

\n {line++; col=1;}
\r {;}
[ \t] {col++;}
. {printf("Line %d, col %d: illegal character (%s)\n", line, col - (int)yyleng + 1, yytext);}

"/*" {BEGIN COMMENT; beginning_col = col; beginning_line = line; col += yyleng;}
<COMMENT><<EOF>> {printf("Line <%d>, col <%d>: unterminated comment\n", beginning_line, beginning_col); BEGIN 0;}
<COMMENT>"*/" {BEGIN 0;}
<COMMENT>. {col++;}
<COMMENT>\n {line++; col=1;}

"//" {BEGIN COMMENT2;}
<COMMENT2>"\n" {BEGIN 0; line++;}
<COMMENT2>. {col++;}

%%
int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (strcmp(argv[1], "-l") == 0) {
      errors = 0;
    }
  }
  else {
    errors = 1;
  }
  yylex();
  return 0;
}
int yywrap() {
  return 1;
}
