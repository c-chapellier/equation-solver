%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

#include "nonlinear_equ_sys_solver.h"

static expression_t *expr_create(uint8_t type, double dvalue, int var, expression_t *left, expression_t *right);
static void expr_print(expression_t *expr);
static equation_t *equ_create(expression_t *lvalue, expression_t *rvalue);

static system_t sys = {0};

static int n_equs = 0;
static int n_vars = 0;

static void sys_add_equ(system_t *sys, equation_t *equ)
{
	sys->equs[n_equs++] = equ;
}

static int sys_register_var(system_t *sys, const char *var)
{
	for (int i = 0; i < n_vars; ++i)
		if (strcmp(sys->vars[i], var) == 0)
			return i;

	sys->vars[n_vars++] = strdup(var);
	return n_vars - 1;
}

static void sys_print(const system_t sys)
{
	printf("System:\n");
	printf("  Equations:\n");
	for (int i = 0; i < n_equs; ++i)
	{
		printf("    ");
		expr_print(sys.equs[i]->lvalue);
		printf(" = ");
		expr_print(sys.equs[i]->rvalue);
		printf("\n");
	}
	printf("  Variables:\n");
	for (int i = 0; i < n_vars; ++i)
		printf("    %s\n", sys.vars[i]);
}

%}

%union {
	double dval;
	char* sval;
	void *exprval;
	void *equval;
}

%token<sval> T_VAR
%token<dval> T_DOUBLE

%token T_EQU T_ADD T_SUB T_MUL T_DIV T_EXP T_LPAR T_RPAR
%token T_NEWLINE
%left T_ADD T_SUB
%left T_MUL T_DIV

%type<exprval> expression
%type<equval> equation

%start prog

%%

prog:
	   | prog line { }
;

line: T_NEWLINE
    | equation T_NEWLINE { sys_add_equ(&sys, $1); }
;

// equation: expression T_EQU expression { expr_print($1); printf(" = "); expr_print($3); printf("\n"); $$ = equ_create($1, $3);}
equation: expression T_EQU expression { $$ = equ_create($1, $3);}
;

expression:
		T_DOUBLE	{ $$ = expr_create(EXPR_TYPE_DOUBLE, $1, -1, NULL, NULL); }
	  | T_VAR		{ int i = sys_register_var(&sys, $1); $$ = expr_create(EXPR_TYPE_VAR, 0, i, NULL, NULL); }
	  | expression T_ADD expression	{ $$ = expr_create(EXPR_TYPE_ADD, 0, -1, $1, $3) }
	  | expression T_SUB expression	{ $$ = expr_create(EXPR_TYPE_SUB, 0, -1, $1, $3)}
	  | expression T_MUL expression	{ $$ = expr_create(EXPR_TYPE_MUL, 0, -1, $1, $3) }
	  | expression T_DIV expression	{ $$ = expr_create(EXPR_TYPE_DIV, 0, -1, $1, $3)}
	  | expression T_EXP expression	{ $$ = expr_create(EXPR_TYPE_EXP, 0, -1, $1, $3)}
	  | T_LPAR expression T_RPAR	{ $$ = expr_create(EXPR_TYPE_PAR, 0, -1, $2, NULL)}
;

%%

static expression_t *expr_create(uint8_t type, double dvalue, int var, expression_t *left, expression_t *right)
{
	expression_t *expr = malloc(sizeof (expression_t));
	expr->type = type;
	expr->dval = dvalue;
	expr->var = var;
	expr->eleft = left;
	expr->eright = right;
	return expr;
}

static void expr_print(expression_t *expr)
{
	switch (expr->type)
	{
		case EXPR_TYPE_DOUBLE:
			printf("%f", expr->dval);
			break;
		case EXPR_TYPE_VAR:
			printf("vars[%d]", expr->var);
			break;
		case EXPR_TYPE_ADD:
			expr_print(expr->eleft);
			printf(" + ");
			expr_print(expr->eright);
			break;
		case EXPR_TYPE_SUB:
			expr_print(expr->eleft);
			printf(" - ");
			expr_print(expr->eright);
			break;
		case EXPR_TYPE_MUL:
			expr_print(expr->eleft);
			printf(" * ");
			expr_print(expr->eright);
			break;
		case EXPR_TYPE_DIV:
			expr_print(expr->eleft);
			printf(" / ");
			expr_print(expr->eright);
			break;
		case EXPR_TYPE_EXP:
			expr_print(expr->eleft);
			printf(" ^ ");
			expr_print(expr->eright);
			break;
		case EXPR_TYPE_PAR:
			printf("(");
			expr_print(expr->eleft);
			printf(")");
			break;
	}
}

static equation_t *equ_create(expression_t *lvalue, expression_t *rvalue)
{
	equation_t *equ = malloc(sizeof (equation_t));
	equ->lvalue = lvalue;
	equ->rvalue = rvalue;
	return equ;
}

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

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	yyin = fopen(argv[1], "r");
	if (yyin == NULL)
	{
		fprintf(stderr, "Can't open file %s\n", argv[1]);
		exit(1);
	}

	do
	{
		yyparse();
	}
	while(!feof(yyin));

	fclose(yyin);

	sys_print(sys);
	if (n_equs != n_vars)
		fprintf(stderr, "Number of equations and variables must be equal\n"), exit(1);
	sys.n = n_equs;

	double res[sys.n];
	nonlinear_equ_sys_solver(sys, res);

	printf("Solution:\n");
	for (int i = 0; i < sys.n; ++i)
		printf("  %s = %f\n", sys.vars[i], res[i]);

	char *fname = malloc(strlen(argv[1]) + 5);
	strcpy(fname, argv[1]);
	strcat(fname, ".res");

	if (save_to_file(fname, sys, res) == -1)
		fprintf(stderr, "Can't save results to file %s\n", fname), exit(1);

	return 0;
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}