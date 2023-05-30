%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

#include "es.hpp"

System sys = System();

%}

%union {
	double dval;
	char* sval;
	void *expval;
}

%token<sval> T_VAR
%token<dval> T_DOUBLE

%token T_EQU T_ADD T_SUB T_MUL T_DIV T_EXP T_LPAR T_RPAR
%token T_NEWLINE T_EOF

%left T_ADD T_SUB
%left T_MUL T_DIV
%left T_EXP

%type<expval> exp
%type<expval> equ

%start prog

%%

prog:
	| prog line 		{ }
;

line:
	  T_NEWLINE
    | equ T_NEWLINE		{ sys.add_equ((Expression *)$1); }
    | equ T_EOF			{ sys.add_equ((Expression *)$1); }
;

equ: exp T_EQU exp		{ $$ = new Expression(EXPR_TYPE_EQU, 0, 0, (Expression *)$1, (Expression *)$3); }
;

exp:
	  T_DOUBLE			{ $$ = new Expression(EXPR_TYPE_DOUBLE, $1, 0, NULL, NULL); }
	| T_VAR				{ $$ = new Expression(EXPR_TYPE_VAR, 0, sys.add_var($1), NULL, NULL); }
	| exp T_ADD exp		{ $$ = new Expression(EXPR_TYPE_ADD, 0, 0, (Expression *)$1, (Expression *)$3) }
	| exp T_SUB exp		{ $$ = new Expression(EXPR_TYPE_SUB, 0, 0, (Expression *)$1, (Expression *)$3) }
	| exp T_MUL exp		{ $$ = new Expression(EXPR_TYPE_MUL, 0, 0, (Expression *)$1, (Expression *)$3) }
	| exp T_DIV exp		{ $$ = new Expression(EXPR_TYPE_DIV, 0, 0, (Expression *)$1, (Expression *)$3) }
	| exp T_EXP exp		{ $$ = new Expression(EXPR_TYPE_EXP, 0, 0, (Expression *)$1, (Expression *)$3) }
	| T_LPAR exp T_RPAR	{ $$ = new Expression(EXPR_TYPE_PAR, 0, 0, (Expression *)$2, NULL) }
;

%%

int main(int argc, char* argv[])
{
	if (argc != 2)
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]), exit(1);

	yyin = fopen(argv[1], "r");
	if (yyin == NULL)
		fprintf(stderr, "Can't open file %s\n", argv[1]), exit(1);

	do
		yyparse();
	while(!feof(yyin));

	fclose(yyin);

	if (DEBUG_MODE) sys.print();

	double res[sys.size()];
	sys.solve(res);

	debug("Solution:\n");
	for (int i = 0; i < sys.size(); ++i)
		debug("  %s = %f\n", sys.vars[i], res[i]);

	char *fname = (char *)malloc(strlen(argv[1]) + 5);
	strcpy(fname, argv[1]);
	strcat(fname, ".res");

	if (sys.save_to_file(fname, res) == -1)
		fprintf(stderr, "Can't save results to file %s\n", fname), exit(1);

	char *fname2 = (char *)malloc(strlen(argv[1]) + 5);
	strcpy(fname2, argv[1]);
	strcat(fname2, ".md");

	if (sys.save_to_markdown(fname2, res) == -1)
		fprintf(stderr, "Can't save results to file %s\n", fname), exit(1);

	return 0;
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
