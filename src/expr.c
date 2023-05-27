
#include "nonlinear_equ_sys_solver.h"

expression_t *expr_create(uint8_t type, double dvalue, int var, expression_t *left, expression_t *right)
{
	expression_t *expr = malloc(sizeof (expression_t));
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
	}
}
