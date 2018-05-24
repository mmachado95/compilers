# meta 1
#lex $1.l
#clang-3.8 -o $1 lex.yy.c
#./$1 -l < $2

lex $1.l
# o -v ou -t gera um ficheiro output com a tabela de parsing
yacc -d -v $1.y
cc -Wall -Wno-unused-function -o $1 y.tab.c lex.yy.c ast.c symbol_table.c semantics.c generate.c
./$1 < $2
zip uccompiler.zip uccompiler.l uccompiler.y y.tab.c y.tab.h ast.c ast.h semantics.c semantics.h symbol_table.c symbol_table.h generate.c generate.h
