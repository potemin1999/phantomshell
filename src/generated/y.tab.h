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
    END_OF_FILE = 258,
    PAREN_OPEN = 259,
    PAREN_CLOSE = 260,
    BRACE_OPEN = 261,
    BRACE_CLOSE = 262,
    yy_EQUAL_TO = 263,
    yy_NOT_EQUAL_TO = 264,
    yy_NOT_LESS_THAN = 265,
    yy_LESS_THAN = 266,
    yy_GREATER_THAN = 267,
    yy_NOT_GREATER_THAN = 268,
    DEF = 269,
    RETURN = 270,
    IF = 271,
    ELIF = 272,
    ELSE = 273,
    FOR = 274,
    WHILE = 275,
    DO = 276,
    SWITCH = 277,
    CASE = 278,
    OTHER = 279,
    CLASS = 280,
    FUNC = 281,
    Identifier = 282,
    BooleanLiteral = 283,
    IntegerLiteral = 284,
    FloatLiteral = 285,
    CharLiteral = 286,
    StringLiteral = 287,
    CALL_PREC = 288,
    UNARY_MINUS_PREC = 289
  };
#endif
/* Tokens.  */
#define END_OF_FILE 258
#define PAREN_OPEN 259
#define PAREN_CLOSE 260
#define BRACE_OPEN 261
#define BRACE_CLOSE 262
#define yy_EQUAL_TO 263
#define yy_NOT_EQUAL_TO 264
#define yy_NOT_LESS_THAN 265
#define yy_LESS_THAN 266
#define yy_GREATER_THAN 267
#define yy_NOT_GREATER_THAN 268
#define DEF 269
#define RETURN 270
#define IF 271
#define ELIF 272
#define ELSE 273
#define FOR 274
#define WHILE 275
#define DO 276
#define SWITCH 277
#define CASE 278
#define OTHER 279
#define CLASS 280
#define FUNC 281
#define Identifier 282
#define BooleanLiteral 283
#define IntegerLiteral 284
#define FloatLiteral 285
#define CharLiteral 286
#define StringLiteral 287
#define CALL_PREC 288
#define UNARY_MINUS_PREC 289

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

#line 135 "y.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
