%option noyywrap

%{

#include "nonlinear_equ_sys_solver.h"

#define YY_DECL int yylex()

#include "parser.h"

%}

%%

\{.*\}                          { debug("comment {}"); }
\"(\\.|[^\"\\])*\"              { debug("comment \"\""); }
[ \t]	                        ; // ignore all whitespace
[a-zA-Z_][a-zA-Z0-9_]{0,31}     { yylval.sval = strdup(yytext); debug("T_VAR"); debug("%s", yylval.sval); return T_VAR; }
[0-9]+\.[0-9]+                  { yylval.dval = atof(yytext); debug("T_DOUBLE"); return T_DOUBLE; }
[0-9]+		                    { yylval.dval = atof(yytext); debug("T_DOUBLE"); return T_DOUBLE; }
\n		                        { debug("T_NEWLINE"); return T_NEWLINE; }
"="		                        { debug("T_EQU"); return T_EQU; }
"+"		                        { debug("T_ADD"); return T_ADD; }
"-"		                        { debug("T_SUB"); return T_SUB; }
"*"		                        { debug("T_MUL"); return T_MUL; }
"/"		                        { debug("T_DIV"); return T_DIV; }
"^"		                        { debug("T_EXP"); return T_EXP; }
"("		                        { debug("T_LPAR"); return T_LPAR; }
")"		                        { debug("T_RPAR"); return T_RPAR; }

%%