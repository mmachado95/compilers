lex $1.l
clang-3.8 -o $1 lex.yy.c
./$1 -l < $2
