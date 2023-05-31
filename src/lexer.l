
%option noyywrap
%option perf-report
%option warn

%{

#include "es.hpp"

#define YY_DECL int yylex()

#include "parser.hpp"

%}

comments        \"(\\.|[^\"\\])*\"
whitespaces     [ \t]
var             [a-zA-Z_][a-zA-Z0-9_]{0,31}
num             ([0-9]+\.[0-9]+)|([0-9]+)

%%

{comments}      ;
{whitespaces}	; // ignore all whitespace
"function"      return T_FUNC;
"return"        return T_RETURN;
{var}           { yylval.sval = strdup(yytext); return T_VAR; }
{num}           { yylval.dval = atof(yytext); return T_DOUBLE; }
\n		        return T_NEWLINE;
"="		        return T_EQU;
"+"		        return T_ADD;
"-"		        return T_SUB;
"*"		        return T_MUL;
"/"		        return T_DIV;
"^"		        return T_EXP;
"("		        return T_LPAR;
")"		        return T_RPAR;
","             return T_COMMA;
"{"             return T_LBRA;
"}"             return T_RBRA;
"<<EOF>>"       return T_EOF;

%%