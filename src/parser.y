%{
#include "../parser.h"
#include "../types.h"
#include "../operator.h"
#include <stdio.h>
#include <string.h>
%}

%union{
	struct ast_node_t *ast;
	bool_t bool_value;
	int_t int_value;
	float_t float_value;
	char_t char_value;
	string_t string_value;
}

%start Program

//SYMBOL TOKENS
// paren ()
// brace {}
// bracket []
%token PAREN_OPEN PAREN_CLOSE BRACE_OPEN BRACE_CLOSE BRACKET_OPEN BRACKET_CLOSE

//KEYWORD TOKENS
%token DEF RETURN IF ELIF ELSE FOR WHILE DO SWITCH CASE OTHER CLASS FUNC

//BASIC TOKENS
%token DigitSequence CharSequence
%token <string_value> 	Identifier TypeSpecifier

//LITERAL TOKENS
%token <bool_value>	BooleanLiteral
%token <int_value> 	IntegerLiteral
%token <float_value> 	FloatLiteral
%token <char_value> 	CharLiteral
%token <string_value>	StringLiteral

//OPERATORS
%right RETURN
%right '='  //prec 14
%left '+' '-' //prec 4
%left '*' '/' //prec 3
%right '!' //prec 2
%left UNARY_MINUS_PREC

%type <ast> Scope
%type <ast> TernaryExpression BinaryExpression UnaryExpression Expression ConstantExpression
%type <ast> Statements Statement SelectionStatement IterationStatement JumpStatement Declaration
%type <ast> switchSelectionBlock elifSelectionBlock
%type <ast> FuncDeclaration FunctionArgs
%%

//BASIC

//EXPRESSIONS
ConstantExpression
	: BooleanLiteral  { $$ = ast_new_node_literal_bool($1);   }
	| IntegerLiteral  { $$ = ast_new_node_literal_int($1);    }
	| FloatLiteral 	  { $$ = ast_new_node_literal_float($1);  }
	| CharLiteral 	  { $$ = ast_new_node_literal_char($1);   }
	| StringLiteral   { $$ = ast_new_node_literal_string($1); }

UnaryExpression
	: '!' Expression  { $$ = ast_new_node_unary_op(LOGICAL_NOT, $2); }
	| '-' Expression  { $$ = ast_new_node_unary_op(UNARY_MINUS, $2); } %prec UNARY_MINUS_PREC

BinaryExpression
	: Expression '=' Expression	{ $$ = ast_new_node_binary_op(ASSIGNMENT, $1, $3); }
	| Expression '=''=' Expression 	{ $$ = ast_new_node_binary_op(EQUAL_TO, $1, $4); }
	| Expression '+' Expression 	{ $$ = ast_new_node_binary_op(ADDITION, $1, $3); }
	| Expression '-' Expression 	{ $$ = ast_new_node_binary_op(SUBTRACTION, $1, $3); }
	| Expression '*' Expression 	{ $$ = ast_new_node_binary_op(MULTIPLICATION, $1, $3); }
	| Expression '/' Expression 	{ $$ = ast_new_node_binary_op(DIVISION, $1, $3); }

TernaryExpression
	: PAREN_OPEN Expression '?' Expression ':' Expression PAREN_CLOSE {
		$$ = ast_new_node_ternary_op(TERNARY_CONDITIONAL, $2, $4, $6);
	}

Expression
	: Identifier 		{ $$ = ast_new_node_ident($1); }
	| UnaryExpression 	{ $$ = $1; }
	| BinaryExpression 	{ $$ = $1; }
	| TernaryExpression 	{ $$ = $1; }
	| ConstantExpression 	{ $$ = $1; }
	| PAREN_OPEN Expression PAREN_CLOSE { $$ = ast_new_node_group($2); }

//STATEMENTS
FLUSH	: ';'
	| '\n'

_FLUSH 	: FLUSH
	|
	;

Scope 	: BRACE_OPEN _FLUSH Statements BRACE_CLOSE 	{ $$ = ast_new_node_scope($3); }
	| BRACE_OPEN _FLUSH BRACE_CLOSE 		{ $$ = ast_new_node_scope(0);  }

Statement : Expression FLUSH	{ $$ = ast_new_node_stat_expr($1); }
	| Scope 		{ $$ = $1; }
	| SelectionStatement 	{ $$ = $1; }
	| IterationStatement 	{ $$ = $1; }
	| JumpStatement 	{ $$ = $1; }
	| Declaration 		{ $$ = $1; }
	| Statement FLUSH 	{ $$ = $1; }

Statements
	: 			{ $$ = ast_new_node_stat_list(0);	}
	| Statements Statement	{ $$ = ast_link_node_stat_list($1,$2); 	}

SelectionStatement
	: IF Expression Scope 				{ $$ = ast_new_node_stat_if($2,$3,0); }
	| IF Expression Scope ELSE Scope	 	{ $$ = ast_new_node_stat_if($2,$3,$5); }
	| IF Expression Scope elifSelectionBlock 	{ $$ = ast_new_node_stat_if($2,$3,$4); }
	| SWITCH Expression BRACE_OPEN _FLUSH
		switchSelectionBlock
	  BRACE_CLOSE 					{ $$ = ast_new_node_stat_switch($2,$5); }

IterationStatement : WHILE Expression Scope 	{ $$ = ast_new_node_stat_while($2,$3); }

JumpStatement : RETURN Expression 	{ $$ = ast_new_node_stat_ret($2); }

switchSelectionBlock
	: switchSelectionBlock
	  OTHER Scope _FLUSH 			{ ast_new_node_stat_switch_choice(0,$3,$1); $$ = $1; }
	| OTHER Scope _FLUSH 			{ $$ = ast_new_node_stat_switch_choice(0,$2,0); }
	| switchSelectionBlock
	  CASE ConstantExpression Scope _FLUSH 	{ ast_new_node_stat_switch_choice($3,$4,$1); $$ = $1; }
	| CASE ConstantExpression Scope _FLUSH	{ $$ = ast_new_node_stat_switch_choice($2,$3,0); }

elifSelectionBlock
	: ELIF Expression Scope				{ $$ = ast_new_node_stat_if($2,$3,0); }
	| ELIF Expression Scope elifSelectionBlock	{ $$ = ast_new_node_stat_if($2,$3,$4); }
	| ELIF Expression Scope ELSE Scope		{ $$ = ast_new_node_stat_if($2,$3,$5); }

//STRUCTURE
FunctionArgs : Identifier Identifier 			{ $$ = ast_new_node_func_arg($1,$2,0); }
	| FunctionArgs ',' Identifier Identifier 	{ ast_new_node_func_arg($3,$4,$1); $$ = $1; }
	| 						{ $$ = 0; }
	;

FuncDeclaration
	: FUNC Identifier PAREN_OPEN FunctionArgs PAREN_CLOSE Scope
		{ $$ = ast_new_node_decl_func($2,$4,0,$6); }
	| FUNC Identifier PAREN_OPEN FunctionArgs PAREN_CLOSE Identifier Scope
		{ $$ = ast_new_node_decl_func($2,$4,$6,$7); }

Declaration : FuncDeclaration {
		printf("Func was defined\n");
	}


Program : Program Statement {
        	//printf("Defined statement in program root\n");
        }
        | Statement {
        	//printf("Defined statement in program root\n");
        }

%%