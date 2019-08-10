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
%token PAREN_OPEN PAREN_CLOSE BRACE_OPEN BRACE_CLOSE
%token yy_EQUAL_TO yy_NOT_EQUAL_TO
%token yy_NOT_LESS_THAN yy_LESS_THAN yy_GREATER_THAN yy_NOT_GREATER_THAN

//KEYWORD TOKENS
%token DEF RETURN IF ELIF ELSE FOR WHILE DO SWITCH CASE OTHER CLASS FUNC

//BASIC TOKENS
%token <string_value> 	Identifier

//LITERAL TOKENS
%token <bool_value>	BooleanLiteral
%token <int_value> 	IntegerLiteral
%token <float_value> 	FloatLiteral
%token <char_value> 	CharLiteral
%token <string_value>	StringLiteral

//OPERATORS
%left RETURN
%left ',' // prec 15
%right '='  //prec 14
%left yy_EQUAL_TO yy_NOT_EQUAL_TO //prec 7
%left yy_NOT_LESS_THAN yy_LESS_THAN yy_GREATER_THAN yy_NOT_GREATER_THAN // prec 6
%left '+' '-' //prec 4
%left '*' '/' //prec 3
%right '!' CALL_PREC //prec 2
%nonassoc PAREN_OPEN PAREN_CLOSE
%left UNARY_MINUS_PREC

%type <ast> Scope GroupExpression
%type <ast> TernaryExpression CommaBinaryExpression BinaryExpression UnaryExpression Expression ConstantExpression
%type <ast> Statements Statement SelectionStatement IterationStatement JumpStatement Declaration
%type <ast> switchSelectionBlock elifSelectionBlock
%type <ast> VarDeclaration VarDeclarations FuncDeclaration
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

CommaBinaryExpression
	: Expression ',' Expression 	{ $$ = ast_new_node_binary_op(COMMA,$1,$3); }

BinaryExpression
	: Expression '=' Expression	{ $$ = ast_new_node_binary_op(ASSIGNMENT, $1, $3); }
	| Expression '+' Expression 	{ $$ = ast_new_node_binary_op(ADDITION, $1, $3); }
	| Expression '-' Expression 	{ $$ = ast_new_node_binary_op(SUBTRACTION, $1, $3); }
	| Expression '*' Expression 	{ $$ = ast_new_node_binary_op(MULTIPLICATION, $1, $3); }
	| Expression '/' Expression 	{ $$ = ast_new_node_binary_op(DIVISION, $1, $3); }
	| Expression GroupExpression	{ $$ = ast_new_node_binary_op(FUNCTION_CALL, $1, $2);	} %prec CALL_PREC
	| Expression yy_EQUAL_TO 	 Expression	{ $$ = ast_new_node_binary_op(EQUAL_TO, $1, $3); }
        | Expression yy_NOT_EQUAL_TO 	 Expression 	{ $$ = ast_new_node_binary_op(NOT_EQUAL_TO, $1, $3); }
        | Expression yy_LESS_THAN 	 Expression	{ $$ = ast_new_node_binary_op(LESS_THAN, $1, $3); 	}
        | Expression yy_NOT_GREATER_THAN Expression 	{ $$ = ast_new_node_binary_op(NOT_GREATER_THAN, $1, $3);}
        | Expression yy_GREATER_THAN 	 Expression 	{ $$ = ast_new_node_binary_op(GREATER_THAN, $1, $3); 	}
        | Expression yy_NOT_LESS_THAN 	 Expression 	{ $$ = ast_new_node_binary_op(NOT_LESS_THAN, $1, $3); 	}

TernaryExpression
	: PAREN_OPEN Expression '?' Expression ':' Expression PAREN_CLOSE {
		$$ = ast_new_node_ternary_op(TERNARY_CONDITIONAL, $2, $4, $6);
	}

GroupExpression
	: PAREN_OPEN Expression PAREN_CLOSE 	{ $$ = ast_new_node_group($2); }


VarDeclaration : Identifier Identifier	{ $$ = ast_new_node_decl_var($2,$1);	}

VarDeclarations
	: VarDeclaration 	{ $$ = $1; }
	| CommaBinaryExpression { $$ = $1; }

Expression
	: Identifier 		{ $$ = ast_new_node_ident($1); }
	| UnaryExpression 	{ $$ = $1; }
	| BinaryExpression 	{ $$ = $1; }
	| TernaryExpression 	{ $$ = $1; }
	| ConstantExpression 	{ $$ = $1; }
	| GroupExpression	{ $$ = $1; }
	| VarDeclaration	{ $$ = $1; }

//STATEMENTS
FLUSH	: ';'
	| '\n'

_FLUSH 	: FLUSH
	|
	;

Scope 	: BRACE_OPEN _FLUSH Statements BRACE_CLOSE	{ $$ = ast_new_node_scope($3); }

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
	: IF Expression Scope  				{ $$ = ast_new_node_stat_if($2,$3,0); }
	| IF Expression Scope ELSE Scope 	 	{ $$ = ast_new_node_stat_if($2,$3,$5); }
	| IF Expression Scope elifSelectionBlock 	{ $$ = ast_new_node_stat_if($2,$3,$4); }
	| SWITCH Expression BRACE_OPEN _FLUSH
		switchSelectionBlock
	  BRACE_CLOSE 					{ $$ = ast_new_node_stat_switch($2,$5); }

IterationStatement : WHILE Expression Scope 	{ $$ = ast_new_node_stat_while($2,$3); }

JumpStatement : RETURN Expression 	{ $$ = ast_new_node_stat_ret($2); } %prec RETURN

switchSelectionBlock
	: switchSelectionBlock
	  OTHER Scope  			{ ast_new_node_stat_switch_choice(0,$3,$1); $$ = $1; }
	| OTHER Scope  			{ $$ = ast_new_node_stat_switch_choice(0,$2,0); }
	| switchSelectionBlock
	  CASE ConstantExpression Scope { ast_new_node_stat_switch_choice($3,$4,$1); $$ = $1; }
	| CASE ConstantExpression Scope { $$ = ast_new_node_stat_switch_choice($2,$3,0); }

elifSelectionBlock
	: ELIF Expression Scope				{ $$ = ast_new_node_stat_if($2,$3,0); }
	| ELIF Expression Scope elifSelectionBlock	{ $$ = ast_new_node_stat_if($2,$3,$4); }
	| ELIF Expression Scope ELSE Scope		{ $$ = ast_new_node_stat_if($2,$3,$5); }

//STRUCTURE

FuncDeclaration
	: FUNC Identifier PAREN_OPEN VarDeclarations PAREN_CLOSE Scope
		{ $$ = ast_new_node_decl_func($2,$4,0,$6); }
	| FUNC Identifier PAREN_OPEN VarDeclarations PAREN_CLOSE Identifier Scope
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