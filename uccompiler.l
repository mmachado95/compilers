%{
  #include "y.tab.h"
  #define PRINTF if(print_tokens == 1) printf
  #include "ast.h"
  #include "symbol_table.h"
  #include "semantics.h"
  int line = 1;
  int col = 1;
  int beginning_line;
  int beginning_col;
  int print_tokens;
  int errors = 0;
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
RESERVED "auto"|"break"|"case"|"const"|"continue"|"default"|"do"|"enum"|"extern"|"float"|"for"|"goto"|"inline"|"long"|"register"|"restrict"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"volatile"|"_Bool"|"_Complex"|"_Imaginary"|"["|"]"|"++"|"--"

NUMBER [0-9]
LETTER [a-zA-Z]
ANYCHAR [^'\n\\]
ESCAPE \\(n|t|\\|\'|\"|([0-7]{1,3}))
EXPOENTE (E|e)(\+|\-)*{NUMBER}+

ID ({LETTER}|_)({LETTER}|{NUMBER}|_)*
INTLIT {NUMBER}+
CHRLIT \'{ANYCHAR}\'|\'{ESCAPE}\'
REALLIT ({NUMBER}+\.{NUMBER}*{EXPOENTE}*)|(\.{NUMBER}+{EXPOENTE}*)|({NUMBER}+{EXPOENTE})
CHRLIT_INV \'(\\.|{ANYCHAR})*\'
CHRLIT_UNT \'({ANYCHAR}|{ESCAPE})*(\\)*{ANYCHAR}*

%%
{CHAR} {PRINTF("CHAR\n"); col += yyleng; return CHAR;}
{ELSE} {PRINTF("ELSE\n"); col += yyleng; return ELSE;}
{WHILE} {PRINTF("WHILE\n"); col += yyleng; return WHILE;}
{IF} {PRINTF("IF\n"); col += yyleng; return IF;}
{INT} {PRINTF("INT\n"); col += yyleng; return INT;}
{SHORT} {PRINTF("SHORT\n"); col += yyleng; return SHORT;}
{DOUBLE} {PRINTF("DOUBLE\n"); col += yyleng; return DOUBLE;}
{RETURN} {PRINTF("RETURN\n"); col += yyleng; return RETURN;}
{VOID} {PRINTF("VOID\n"); col += yyleng; return VOID;}
{BITWISEAND} {PRINTF("BITWISEAND\n"); col += yyleng; return BITWISEAND;}
{BITWISEOR} {PRINTF("BITWISEOR\n"); col += yyleng; return BITWISEOR;}
{BITWISEXOR} {PRINTF("BITWISEXOR\n"); col += yyleng; return BITWISEXOR;}
{AND} {PRINTF("AND\n"); col += yyleng; return AND;}
{ASSIGN} {PRINTF("ASSIGN\n"); col += yyleng; return ASSIGN;}
{MUL} {PRINTF("MUL\n"); col += yyleng; return MUL;}
{COMMA} {PRINTF("COMMA\n"); col += yyleng; return COMMA;}
{DIV} {PRINTF("DIV\n"); col += yyleng; return DIV;}
{EQ} {PRINTF("EQ\n"); col += yyleng; return EQ;}
{GE} {PRINTF("GE\n"); col += yyleng; return GE;}
{GT} {PRINTF("GT\n"); col += yyleng; return GT;}
{LBRACE} {PRINTF("LBRACE\n"); col += yyleng; return LBRACE;}
{LE} {PRINTF("LE\n"); col += yyleng; return LE;}
{LPAR} {PRINTF("LPAR\n"); col += yyleng; return LPAR;}
{LT} {PRINTF("LT\n"); col += yyleng; return LT;}
{MINUS} {PRINTF("MINUS\n"); col += yyleng; return MINUS;}
{MOD} {PRINTF("MOD\n"); col += yyleng; return MOD;}
{NE} {PRINTF("NE\n"); col += yyleng; return NE;}
{NOT} {PRINTF("NOT\n"); col += yyleng; return NOT;}
{OR} {PRINTF("OR\n"); col += yyleng; return OR;}
{PLUS} {PRINTF("PLUS\n"); col += yyleng; return PLUS;}
{RBRACE} {PRINTF("RBRACE\n"); col += yyleng; return RBRACE;}
{RPAR} {PRINTF("RPAR\n"); col += yyleng; return RPAR;}
{SEMI} {PRINTF("SEMI\n"); col += yyleng; return SEMI;}
{RESERVED} {PRINTF("RESERVED(%s)\n", yytext); col += yyleng; return RESERVED;}

{INTLIT} {PRINTF("INTLIT(%s)\n", yytext); col += yyleng; yylval.value = strdup(yytext); return INTLIT;}
{ID} {PRINTF("ID(%s)\n", yytext); col += yyleng; yylval.value = strdup(yytext); return ID;}
{CHRLIT} {PRINTF("CHRLIT(%s)\n", yytext); col += yyleng; yylval.value = strdup(yytext); return CHRLIT;}
{REALLIT} {PRINTF("REALLIT(%s)\n", yytext); col += yyleng; yylval.value = strdup(yytext); return REALLIT;}
{CHRLIT_INV} {printf("Line %d, col %d: invalid char constant (%s)\n", line, col, yytext); col+=yyleng; return CHRLIT_INV;}
{CHRLIT_UNT} {printf("Line %d, col %d: unterminated char constant\n", line, col); col+=yyleng; return CHRLIT_UNT;}

\n {line++; col=1;}
\r {col+=yyleng;}
[ \t] {col+=yyleng;}
. {printf("Line %d, col %d: illegal character (%s)\n", line, col, yytext); col+=yyleng;}


"/*" {BEGIN COMMENT; beginning_col = col; beginning_line = line; col+=yyleng;}
<COMMENT><<EOF>> {BEGIN 0; printf("Line %d, col %d: unterminated comment\n", beginning_line, beginning_col); col+=yyleng;}
<COMMENT>"*/" {BEGIN 0; col+=yyleng;}
<COMMENT>. {col+=yyleng;}
<COMMENT>\n {line++; col=1;}

"//" {BEGIN COMMENT2;}
<COMMENT2>"\n" {BEGIN 0; line++; col=1;}
<COMMENT2>. {;}

%%
int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (strcmp(argv[1], "-l") == 0) {
      print_tokens = 1;
      yyparse();
      yylex();
    }
    else if (strcmp(argv[1], "-2") == 0) {
      print_tokens = 0;
      yyparse();
      destroy_ast(ast);
    }
    else if (strcmp(argv[1], "-t") == 0) {
      print_tokens = 0;
      yyparse();

      if(errors == 0) {
        print_ast(ast, 0);
        /*current = create_table("Global");*/
        /*insert_default_functions(current);*/
        /*check_program(ast);*/
        /*show_tables();*/
        destroy_ast(ast);
      }
    }
    else if (strcmp(argv[1], "-s") == 0) {
      print_tokens = 0;
      yyparse();

      if(errors == 0) {
        current = create_table("Global");
        current->print = 1;
        insert_default_functions(current);
        check_program(ast);
        show_tables();
        print_ast(ast, 0);
        destroy_ast(ast);
      }
    }
  }
  else {
    print_tokens = 0;
    yyparse();
  }
  return 0;
}

int yywrap() {
  return 1;
}

void yyerror (char *s) {
  errors = 1;
  int aux_col = col - strlen(yytext);
  printf("Line %d, col %d: %s: %s\n", line, aux_col, s, yytext);
}
