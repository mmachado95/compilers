# meta 1
#lex $1.l
#clang-3.8 -o $1 lex.yy.c
#./$1 -l < $2

lex $1.l
# o -v ou -t gera um ficheiro output com a tabela de parsing
yacc -d -v $1.y
cc -Wall -Wno-unused-function -o $1 y.tab.c ast.c symbol_table.c lex.yy.c
./$1 -t < $2

