
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
"function"      { debug("T_FUNC\n"); return T_FUNC; }
"return"        { debug("T_RETURN\n"); return T_RETURN; }
{var}           { yylval.sval = strdup(yytext); debug("T_VAR(%s)\n", yylval.sval); return T_VAR; }
{num}           { yylval.dval = atof(yytext); debug("T_VAR(%f)\n", yylval.dval); return T_DOUBLE; }
\n		        { debug("T_NEWLINE\n"); return T_NEWLINE; }
"="		        { debug("T_EQU\n"); return T_EQU; }
"+"		        { debug("T_ADD\n"); return T_ADD; }
"-"		        { debug("T_SUB\n"); return T_SUB; }
"*"		        { debug("T_MUL\n"); return T_MUL; }
"/"		        { debug("T_DIV\n"); return T_DIV; }
"^"		        { debug("T_EXP\n"); return T_EXP; }
"("		        { debug("T_LPAR\n"); return T_LPAR; }
")"		        { debug("T_RPAR\n"); return T_RPAR; }
","             { debug("T_COMMA\n"); return T_COMMA; }
"{"             { debug("T_LBRA\n"); return T_LBRA; }
"}"             { debug("T_RBRA\n"); return T_RBRA; }
"<<EOF>>"       { debug("T_EOF\n"); return T_EOF; }

%%