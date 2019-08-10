/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
    PAREN_OPEN = 258,
    PAREN_CLOSE = 259,
    BRACE_OPEN = 260,
    BRACE_CLOSE = 261,
    yy_EQUAL_TO = 262,
    yy_NOT_EQUAL_TO = 263,
    yy_NOT_LESS_THAN = 264,
    yy_LESS_THAN = 265,
    yy_GREATER_THAN = 266,
    yy_NOT_GREATER_THAN = 267,
    DEF = 268,
    RETURN = 269,
    IF = 270,
    ELIF = 271,
    ELSE = 272,
    FOR = 273,
    WHILE = 274,
    DO = 275,
    SWITCH = 276,
    CASE = 277,
    OTHER = 278,
    CLASS = 279,
    FUNC = 280,
    Identifier = 281,
    BooleanLiteral = 282,
    IntegerLiteral = 283,
    FloatLiteral = 284,
    CharLiteral = 285,
    StringLiteral = 286,
    CALL_PREC = 287,
    UNARY_MINUS_PREC = 288
  };
#endif
/* Tokens.  */
#define PAREN_OPEN 258
#define PAREN_CLOSE 259
#define BRACE_OPEN 260
#define BRACE_CLOSE 261
#define yy_EQUAL_TO 262
#define yy_NOT_EQUAL_TO 263
#define yy_NOT_LESS_THAN 264
#define yy_LESS_THAN 265
#define yy_GREATER_THAN 266
#define yy_NOT_GREATER_THAN 267
#define DEF 268
#define RETURN 269
#define IF 270
#define ELIF 271
#define ELSE 272
#define FOR 273
#define WHILE 274
#define DO 275
#define SWITCH 276
#define CASE 277
#define OTHER 278
#define CLASS 279
#define FUNC 280
#define Identifier 281
#define BooleanLiteral 282
#define IntegerLiteral 283
#define FloatLiteral 284
#define CharLiteral 285
#define StringLiteral 286
#define CALL_PREC 287
#define UNARY_MINUS_PREC 288

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 9 "../parser.y" /* yacc.c:1921  */

	struct ast_node_t *ast;
	bool_t bool_value;
	int_t int_value;
	float_t float_value;
	char_t char_value;
	string_t string_value;

#line 133 "y.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
