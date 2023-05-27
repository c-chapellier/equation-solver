%option noyywrap

%{
#include <stdio.h>

#define YY_DECL int yylex()

#include "es.tab.h"

%}

%%

[ \t]	; // ignore all whitespace
[a-zA-Z_][a-zA-Z0-9_]{0,31}     { yylval.sval = strdup(yytext); return T_VAR; }
[0-9]+\.[0-9]+                  { yylval.dval = atof(yytext); return T_DOUBLE; }
[0-9]+		                    { yylval.dval = atof(yytext); return T_DOUBLE; }
\n		                        { return T_NEWLINE; }
"="		                        { return T_EQU; }
"+"		                        { return T_ADD; }
"-"		                        { return T_SUB; }
"*"		                        { return T_MUL; }
"/"		                        { return T_DIV; }
"^"		                        { return T_EXP; }
"("		                        { return T_LPAR; }
")"		                        { return T_RPAR; }

%%