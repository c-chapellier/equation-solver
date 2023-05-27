
#include "nonlinear_equ_sys_solver.h"

equation_t *equ_create(expression_t *lvalue, expression_t *rvalue)
{
	equation_t *equ = malloc(sizeof (equation_t));
	equ->lvalue = lvalue;
	equ->rvalue = rvalue;
	return equ;
}
