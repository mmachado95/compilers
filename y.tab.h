/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CHAR = 258,
    ELSE = 259,
    IF = 260,
    INT = 261,
    SHORT = 262,
    DOUBLE = 263,
    RETURN = 264,
    VOID = 265,
    BITWISEAND = 266,
    BITWISEOR = 267,
    BITWISEXOR = 268,
    AND = 269,
    ASSIGN = 270,
    MUL = 271,
    COMMA = 272,
    DIV = 273,
    EQ = 274,
    GE = 275,
    GT = 276,
    LBRACE = 277,
    LE = 278,
    LPAR = 279,
    LT = 280,
    MINUS = 281,
    MOD = 282,
    NE = 283,
    NOT = 284,
    OR = 285,
    PLUS = 286,
    RESERVED = 287,
    RBRACE = 288,
    RPAR = 289,
    SEMI = 290,
    CHRLIT_INV = 291,
    CHRLIT_UNT = 292,
    ID = 293,
    INTLIT = 294,
    CHRLIT = 295,
    REALLIT = 296,
    WHILE = 297,
    THEN = 298
  };
#endif
/* Tokens.  */
#define CHAR 258
#define ELSE 259
#define IF 260
#define INT 261
#define SHORT 262
#define DOUBLE 263
#define RETURN 264
#define VOID 265
#define BITWISEAND 266
#define BITWISEOR 267
#define BITWISEXOR 268
#define AND 269
#define ASSIGN 270
#define MUL 271
#define COMMA 272
#define DIV 273
#define EQ 274
#define GE 275
#define GT 276
#define LBRACE 277
#define LE 278
#define LPAR 279
#define LT 280
#define MINUS 281
#define MOD 282
#define NE 283
#define NOT 284
#define OR 285
#define PLUS 286
#define RESERVED 287
#define RBRACE 288
#define RPAR 289
#define SEMI 290
#define CHRLIT_INV 291
#define CHRLIT_UNT 292
#define ID 293
#define INTLIT 294
#define CHRLIT 295
#define REALLIT 296
#define WHILE 297
#define THEN 298

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 13 "uccompiler.y" /* yacc.c:1915  */

  char* value;
  struct node *node;

#line 145 "y.tab.h" /* yacc.c:1915  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
