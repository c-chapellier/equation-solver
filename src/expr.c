
#include "nonlinear_equ_sys_solver.h"

expression_t *expr_create(uint8_t type, double dvalue, int var, expression_t *left, expression_t *right)
{
	expression_t *expr = malloc(sizeof(expression_t));
	expr->type = type;
	expr->dval = dvalue;
	expr->var = var;
	expr->eleft = left;
	expr->eright = right;
	return expr;
}

void expr_print(expression_t *expr)
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
	case EXPR_TYPE_EQU:
		expr_print(expr->eleft);
		printf(" = ");
		expr_print(expr->eright);
		break;
	}
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

static void var_to_latex(FILE *f, const char *var)
{
	char *s = strchr(var, '_');
	if (s == NULL)
		fprintf(f, "%s", var);
	else
		fprintf(f, "%.*s_{%s}", (int)(s - var), var, s + 1);
}

void expr_to_latex(FILE *f, system_t sys, expression_t *expr)
{
	switch (expr->type)
	{
	case EXPR_TYPE_DOUBLE:
		double_to_latex(f, expr->dval);
		break;
	case EXPR_TYPE_VAR:
		var_to_latex(f, sys.vars[expr->var]);
		break;
	case EXPR_TYPE_ADD:
		expr_to_latex(f, sys, expr->eleft);
		fprintf(f, " + ");
		expr_to_latex(f, sys, expr->eright);
		break;
	case EXPR_TYPE_SUB:
		expr_to_latex(f, sys, expr->eleft);
		fprintf(f, " - ");
		expr_to_latex(f, sys, expr->eright);
		break;
	case EXPR_TYPE_MUL:
		expr_to_latex(f, sys, expr->eleft);
		fprintf(f, " \\cdot ");
		expr_to_latex(f, sys, expr->eright);
		break;
	case EXPR_TYPE_DIV:
		fprintf(f, "\\frac{");
		expr_to_latex(f, sys, expr->eleft);
		fprintf(f, "}{");
		expr_to_latex(f, sys, expr->eright);
		fprintf(f, "}");
		break;
	case EXPR_TYPE_EXP:
		expr_to_latex(f, sys, expr->eleft);
		fprintf(f, "^{");
		expr_to_latex(f, sys, expr->eright);
		fprintf(f, "}");
		break;
	case EXPR_TYPE_PAR:
		fprintf(f, "\\left(");
		expr_to_latex(f, sys, expr->eleft);
		fprintf(f, "\\right)");
		break;
	case EXPR_TYPE_EQU:
		expr_to_latex(f, sys, expr->eleft);
		fprintf(f, " = ");
		expr_to_latex(f, sys, expr->eright);
		break;
	}
}
