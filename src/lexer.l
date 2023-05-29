%option noyywrap

%{

#include "nonlinear_equ_sys_solver.h"

#define YY_DECL int yylex()

#include "parser.h"

%}

%%

\{.*\}                          { debug("comment {}\n"); }
\"(\\.|[^\"\\])*\"              { debug("comment \"\"\n"); }
[ \t]	                        ; // ignore all whitespace
[a-zA-Z_][a-zA-Z0-9_]{0,31}     { yylval.sval = strdup(yytext); debug("T_VAR (%s)\n", yylval.sval); return T_VAR; }
[0-9]+\.[0-9]+                  { yylval.dval = atof(yytext); debug("T_DOUBLE (%f)\n", yylval.dval); return T_DOUBLE; }
[0-9]+		                    { yylval.dval = atof(yytext); debug("T_DOUBLE (%f)\n", yylval.dval); return T_DOUBLE; }
\n		                        { debug("T_NEWLINE\n"); return T_NEWLINE; }
"="		                        { debug("T_EQU\n"); return T_EQU; }
"+"		                        { debug("T_ADD\n"); return T_ADD; }
"-"		                        { debug("T_SUB\n"); return T_SUB; }
"*"		                        { debug("T_MUL\n"); return T_MUL; }
"/"		                        { debug("T_DIV\n"); return T_DIV; }
"^"		                        { debug("T_EXP\n"); return T_EXP; }
"("		                        { debug("T_LPAR\n"); return T_LPAR; }
")"		                        { debug("T_RPAR\n"); return T_RPAR; }
"<<EOF>>"                       { debug("T_EOF\n"); return T_EOF; }

%%