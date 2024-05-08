
%option noyywrap
%option perf-report
%option warn
%option yylineno

%{

#include "es.hpp"

#define YY_DECL int yylex()

#include "parser.hpp"

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

%}

comments        \"(\\.|[^\"\\])*\"
whitespaces     [ \t]
var             [a-zA-Z_][a-zA-Z0-9_]{0,31}
guess           \{[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?\}
num             [0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?
unit            \[([a-zA-Z]{0,31}-?[0-9]?\ ?)+\]

%%

{comments}      ;
{whitespaces}	; // ignore all whitespace
{guess}         { yylval.sval = strdup(yytext); debug("T_GUESS(" + std::string(yylval.sval) + ")"); return T_GUESS; }
{num}           { yylval.dval = atof(yytext); debug("T_NUM(" + std::to_string(yylval.dval) + ")"); return T_DOUBLE; }
{unit}          { yylval.sval = strdup(yytext); debug("T_UNIT(" + std::string(yylval.sval) + ")"); return T_UNIT; }
"function"      { debug("T_FUNC"); return T_FUNC; }
"return"        { debug("T_RETURN"); return T_RETURN; }
{var}           { yylval.sval = strdup(yytext); debug("T_VAR(" + std::string(yylval.sval) + ")"); return T_VAR; }
\n		        { debug("T_NEWLINE"); return T_NEWLINE; }
"="		        { debug("T_EQU"); return T_EQU; }
"+"		        { debug("T_ADD"); return T_ADD; }
"-"		        { debug("T_SUB"); return T_SUB; }
"*"		        { debug("T_MUL"); return T_MUL; }
"/"		        { debug("T_DIV"); return T_DIV; }
"^"		        { debug("T_POW"); return T_POW; }
"("		        { debug("T_LPAR"); return T_LPAR; }
")"		        { debug("T_RPAR"); return T_RPAR; }
","             { debug("T_COMMA"); return T_COMMA; }
"{"             { debug("T_LBRA"); return T_LBRA; }
"}"             { debug("T_RBRA"); return T_RBRA; }
"<<EOF>>"       { debug("T_EOF"); return T_EOF; }

%%