
#include "System.hpp"

System::System()
{
	this->n = 0;
	this->n_equs = 0;
	this->n_vars = 0;
}

void System::add_equ(Expression *equ)
{
	this->equs[this->n_equs++] = equ;
}

int System::register_var(const char *var)
{
	for (int i = 0; i < this->n_vars; ++i)
		if (strcmp(this->vars[i], var) == 0)
			return i;

	this->vars[this->n_vars++] = strdup(var);
	return this->n_vars - 1;
}

void System::print()
{
	printf("System:\n");
	printf("  Equations:\n");
	for (int i = 0; i < this->n_equs; ++i)
	{
		printf("    ");
		this->equs[i]->print();
		printf("\n");
	}
	printf("  Variables:\n");
	for (int i = 0; i < this->n_vars; ++i)
		printf("    %s\n", this->vars[i]);
}

int System::rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f)
{
    System *sys = (System *)params;

    double y[sys->n];
    for (int i = 0; i < sys->n; ++i)
        y[i] = sys->equs[i]->eval(x);

    for (int i = 0; i < sys->n; ++i)
        gsl_vector_set(f, i, y[i]);

    return GSL_SUCCESS;
}

void System::print_state(size_t iter, int n, gsl_multiroot_fsolver *s)
{
    printf("iter %3zu x = ", iter);
    for (int i = 0; i < n; i++)
        printf("% .3f ", gsl_vector_get(s->x, i));
    printf("f(x) = ");
    for (int i = 0; i < n; i++)
        printf("% .3e ", gsl_vector_get(s->f, i));
    printf("\n");
}

int System::solve(double *res)
{    
    const gsl_multiroot_fsolver_type *T = gsl_multiroot_fsolver_hybrids;
    gsl_multiroot_fsolver *s = gsl_multiroot_fsolver_alloc(T, this->n);
    gsl_multiroot_function f = { &System::rosenbrock_f, (size_t)this->n, this };
    
    gsl_vector *x = gsl_vector_alloc(this->n);
    for (int i = 0; i < this->n; i++)
        gsl_vector_set(x, i, 0.0); // 3rd arg is initial guess

    gsl_multiroot_fsolver_set(s, &f, x);

    int status = GSL_CONTINUE;
    size_t iter = 0;

    if (DEBUG_MODE) print_state(iter, this->n, s);
    while (status == GSL_CONTINUE && iter++ < 1000)
    {
        status = gsl_multiroot_fsolver_iterate(s);
        if (DEBUG_MODE) print_state(iter, this->n, s);
        if (status) /* check if solver is stuck */
            break;
        status = gsl_multiroot_test_residual(s->f, 1e-7);
    }

    debug("status = %s\n", gsl_strerror(status));

    for (int i = 0; i < this->n; i++)
        res[i] = gsl_vector_get(s->x, i);

    gsl_multiroot_fsolver_free(s);
    gsl_vector_free(x);
    return 0;
}