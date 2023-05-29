%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

#include "nonlinear_equ_sys_solver.h"

system_t sys = {0};
int n_equs = 0;
int n_vars = 0;

%}

%union {
	double dval;
	char* sval;
	void *expval;
	void *equval;
}

%token<sval> T_VAR
%token<dval> T_DOUBLE

%token T_EQU T_ADD T_SUB T_MUL T_DIV T_EXP T_LPAR T_RPAR
%token T_NEWLINE T_EOF

%left T_ADD T_SUB
%left T_MUL T_DIV
%left T_EXP

%type<expval> exp
%type<equval> equ

%start prog

%%

prog:
	| prog line 		{ }
;

line:
	  T_NEWLINE
    | equ T_NEWLINE		{ sys_add_equ(&sys, $1); }
    | equ T_EOF			{ sys_add_equ(&sys, $1); }
;

equ: exp T_EQU exp		{ $$ = equ_create($1, $3);}
;

exp:
	  T_DOUBLE			{ $$ = expr_create(EXPR_TYPE_DOUBLE, $1, 0, NULL, NULL); }
	| T_VAR				{ $$ = expr_create(EXPR_TYPE_VAR, 0, sys_register_var(&sys, $1), NULL, NULL); }
	| exp T_ADD exp		{ $$ = expr_create(EXPR_TYPE_ADD, 0, 0, $1, $3) }
	| exp T_SUB exp		{ $$ = expr_create(EXPR_TYPE_SUB, 0, 0, $1, $3) }
	| exp T_MUL exp		{ $$ = expr_create(EXPR_TYPE_MUL, 0, 0, $1, $3) }
	| exp T_DIV exp		{ $$ = expr_create(EXPR_TYPE_DIV, 0, 0, $1, $3) }
	| exp T_EXP exp		{ $$ = expr_create(EXPR_TYPE_EXP, 0, 0, $1, $3) }
	| T_LPAR exp T_RPAR	{ $$ = expr_create(EXPR_TYPE_PAR, 0, 0, $2, NULL) }
;

%%

static int save_to_file(const char *fname, system_t sys, double *res)
{
	FILE *f = fopen(fname, "w");
	if (f == NULL)
		return -1;

	for (int i = 0; i < sys.n; ++i)
		fprintf(f, "%s = %f\n", sys.vars[i], res[i]);

	fclose(f);
	return 0;
}

static void var_to_latex(FILE *f, const char *var)
{
	char *s = strchr(var, '_');
	if (s == NULL)
		fprintf(f, "%s", var);
	else
		fprintf(f, "%.*s_{%s}", (int)(s - var), var, s + 1);
}

static void double_to_latex(FILE *f, double n)
{
	char s[50];
	sprintf(s, "%.10g", n);

	int i = strlen(s);
	while (s[i - 1] == '0')
		i--;
	if (s[i - 1] == '.')
		i--;

	s[i] = '\0';

	fprintf(f, "%s", s);
}

static int save_to_markdown(const char *fname, system_t sys, double *res)
{
	FILE *f = fopen(fname, "w");
	if (f == NULL)
		return -1;

	fprintf(f, "# %s\n\n", fname);
	fprintf(f, "## System of equations\n\n");
	for (int i = 0; i < sys.n; ++i)
	{
		fprintf(f, "$$");
		equ_to_latex(f, sys, sys.equs[i]);
		fprintf(f, "$$\n\n");
	}

	fprintf(f, "## Solution\n\n");
	for (int i = 0; i < sys.n; ++i)
	{
		fprintf(f, "$$");
		var_to_latex(f, sys.vars[i]);
		fprintf(f, " = ");
		double_to_latex(f, res[i]);
		fprintf(f, "$$\n\n");
	}

	fclose(f);
	return 0;
}

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

	if (DEBUG_MODE) sys_print(sys);
	if (n_equs != n_vars)
		fprintf(stderr, "Number of equationss and variables must be equal\n"), exit(1);
	sys.n = n_equs;

	double res[sys.n];
	nonlinear_equ_sys_solver(sys, res);

	debug("Solution:\n");
	for (int i = 0; i < sys.n; ++i)
		debug("  %s = %f\n", sys.vars[i], res[i]);

	char *fname = malloc(strlen(argv[1]) + 5);
	strcpy(fname, argv[1]);
	strcat(fname, ".res");

	if (save_to_file(fname, sys, res) == -1)
		fprintf(stderr, "Can't save results to file %s\n", fname), exit(1);

	char *fname2 = malloc(strlen(argv[1]) + 5);
	strcpy(fname2, argv[1]);
	strcat(fname2, ".md");

	if (save_to_markdown(fname2, sys, res) == -1)
		fprintf(stderr, "Can't save results to file %s\n", fname), exit(1);

	return 0;
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
