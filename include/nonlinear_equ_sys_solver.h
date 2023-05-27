
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

#define DEBUG_MODE 0
// #define DEBUG_MODE 1

#define debug(fmt, ...) \
            do { if (DEBUG_MODE) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)

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
	char *name;
	char **args;
	int n_args;
	expression_t *expr;
} function_t;

typedef struct
{
    int n;
	equation_t *equs[100];
    char *vars[100];
} system_t;

extern system_t sys;
extern int n_equs;
extern int n_vars;

int nonlinear_equ_sys_solver(system_t sys, double *res);

equation_t *equ_create(expression_t *lvalue, expression_t *rvalue);

expression_t *expr_create(uint8_t type, double dvalue, int var, expression_t *left, expression_t *right);
void expr_print(expression_t *expr);

void sys_add_equ(system_t *sys, equation_t *equ);
int sys_register_var(system_t *sys, const char *var);
void sys_print(const system_t sys);
