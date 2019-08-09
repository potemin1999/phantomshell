%{
#include <stdio.h>
#include "../parser.h"
#include "../lexer.h"
#include "y.tab.h"
%}

%%

def     {TRACE_TOKEN("DEF")     return DEF;}
return  {TRACE_TOKEN("RETURN")  return RETURN;}
if      return lexer_handle_keyword_if(); //{TRACE_TOKEN("IF")      return IF;}
elif    {TRACE_TOKEN("ELIF")    return ELIF;}
else    {TRACE_TOKEN("ELSE")    return ELSE;}
for     {TRACE_TOKEN("FOR")     return FOR;}
while   return lexer_handle_keyword_while();//{TRACE_TOKEN("WHILE")   return WHILE;}
do      {TRACE_TOKEN("DO")      return DO;}
switch  return lexer_handle_keyword_switch();//{TRACE_TOKEN("SWITCH")  return SWITCH;}
case    {TRACE_TOKEN("CASE")    return CASE;}
other   {TRACE_TOKEN("OTHER")   return OTHER;}
class   return lexer_handle_keyword_class();
func    return lexer_handle_keyword_func();

true|false              return lexer_handle_bool(yytext);
[0-9]+                  return lexer_handle_int(yytext);
[0-9]+"."[0-9]+         return lexer_handle_float(yytext);
\'[a-zA-Z0-9]\'         return lexer_handle_char(yytext);
["][^"]*["]             return lexer_handle_string(yytext);

[_a-zA-Z][_a-zA-Z0-9]*    return lexer_handle_identifier(yytext);

"=="    return yy_EQUAL_TO;
"!="    return yy_NOT_EQUAL_TO;
">="    return yy_NOT_LESS_THAN;
"<"     return yy_LESS_THAN;
">"     return yy_GREATER_THAN;
"<="    return yy_NOT_GREATER_THAN;

"("     {TRACE_TOKEN("PAREN OPEN")      return PAREN_OPEN;}
")"     {TRACE_TOKEN("PAREN CLOSE")     return PAREN_CLOSE;}
"{"     {TRACE_TOKEN("BRACE OPEN")      return BRACE_OPEN;}
"}"     {TRACE_TOKEN("BRACE CLOSE")     return BRACE_CLOSE;}

"//".*[\n]          {}

\n      {if (use_newline_as_flush) return *yytext; else return yylex();}
[ \t]+
.       return *yytext;

%%