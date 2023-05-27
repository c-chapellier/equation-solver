
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>

#define EXPR_TYPE_DOUBLE 0
#define EXPR_TYPE_VAR 1
#define EXPR_TYPE_ADD 2
#define EXPR_TYPE_SUB 3
#define EXPR_TYPE_MUL 4
#define EXPR_TYPE_DIV 5
#define EXPR_TYPE_EXP 6
#define EXPR_TYPE_PAR 7

typedef struct s_expression
{
	uint8_t type;
	double dval;
	int var;
	struct s_expression *eleft, *eright;
} expression_t;

typedef struct
{
	expression_t *lvalue, *rvalue;
} equation_t;


typedef struct
{
    int n;
	equation_t *equs[100];
    char *vars[100];
} system_t;

int nonlinear_equ_sys_solver(system_t sys, double *res);
