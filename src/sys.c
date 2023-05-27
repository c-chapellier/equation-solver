
#include "nonlinear_equ_sys_solver.h"

void sys_add_equ(system_t *sys, equation_t *equ)
{
	sys->equs[n_equs++] = equ;
}

int sys_register_var(system_t *sys, const char *var)
{
	for (int i = 0; i < n_vars; ++i)
		if (strcmp(sys->vars[i], var) == 0)
			return i;

	sys->vars[n_vars++] = strdup(var);
	return n_vars - 1;
}

void sys_print(const system_t sys)
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
