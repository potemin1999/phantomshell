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
    BRACKET_OPEN = 262,
    BRACKET_CLOSE = 263,
    DEF = 264,
    RETURN = 265,
    IF = 266,
    ELIF = 267,
    ELSE = 268,
    FOR = 269,
    WHILE = 270,
    DO = 271,
    SWITCH = 272,
    CASE = 273,
    OTHER = 274,
    CLASS = 275,
    FUNC = 276,
    DigitSequence = 277,
    CharSequence = 278,
    Identifier = 279,
    TypeSpecifier = 280,
    BooleanLiteral = 281,
    IntegerLiteral = 282,
    FloatLiteral = 283,
    CharLiteral = 284,
    StringLiteral = 285
  };
#endif
/* Tokens.  */
#define PAREN_OPEN 258
#define PAREN_CLOSE 259
#define BRACE_OPEN 260
#define BRACE_CLOSE 261
#define BRACKET_OPEN 262
#define BRACKET_CLOSE 263
#define DEF 264
#define RETURN 265
#define IF 266
#define ELIF 267
#define ELSE 268
#define FOR 269
#define WHILE 270
#define DO 271
#define SWITCH 272
#define CASE 273
#define OTHER 274
#define CLASS 275
#define FUNC 276
#define DigitSequence 277
#define CharSequence 278
#define Identifier 279
#define TypeSpecifier 280
#define BooleanLiteral 281
#define IntegerLiteral 282
#define FloatLiteral 283
#define CharLiteral 284
#define StringLiteral 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 9 "parser.y" /* yacc.c:1921  */

	struct ast_node_t *ast;
	bool_t bool_value;
	int_t int_value;
	float_t float_value;
	char_t char_value;
	string_t string_value;

#line 127 "y.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
