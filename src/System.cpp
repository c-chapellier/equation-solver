
#include "System.hpp"

System::System(int sys_num)
{
	this->sys_num = sys_num;
	this->equs = std::vector<Expression *>();
	this->vars = std::vector<std::string>();
}

int System::size() const
{
	return this->equs.size();
}

void System::add_equ(Expression *equ)
{
	this->equs.push_back(equ);
}

void System::add_var(std::string var)
{
	for (int i = 0; i < this->vars.size(); ++i)
		if (this->vars[i].compare(var) == 0)
			return;

	this->vars.push_back(var);
}

void System::add_sys(System *sys)
{
	for (int i = 0; i < sys->equs.size(); ++i)
		this->add_equ(sys->equs[i]);
	for (int i = 0; i < sys->vars.size(); ++i)
		this->add_var(sys->vars[i]);
}

void System::print() const
{
	printf("System:\n");
	printf("  Equations(%lu):\n", this->equs.size());
	for (int i = 0; i < this->equs.size(); ++i)
	{
		printf("    ");
		this->equs[i]->print();
		printf("\n");
	}
	printf("  Variables(%lu):\n", this->vars.size());
	for (int i = 0; i < this->vars.size(); ++i)
		printf("    %s\n", this->vars[i].c_str());
}

int System::rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f)
{
    System *sys = (System *)params;

    double y[sys->equs.size()];
    for (int i = 0; i < sys->equs.size(); ++i)
        y[i] = sys->equs[i]->eval(sys, x);

    for (int i = 0; i < sys->equs.size(); ++i)
        gsl_vector_set(f, i, y[i]);

    return GSL_SUCCESS;
}

void System::print_state(size_t iter, int n, gsl_multiroot_fsolver *s)
{
	printf("[%d] iter = %3lu x = ", this->sys_num, iter);
	for (int i = 0; i < n; ++i)
		printf("% .3f ", gsl_vector_get(s->x, i));
	printf("f(x) = ");
	for (int i = 0; i < n; ++i)
		printf("% .3e ", gsl_vector_get(s->f, i));
	printf("\n");
}

int System::solve(double *res)
{
    const gsl_multiroot_fsolver_type *T = gsl_multiroot_fsolver_hybrids;
    gsl_multiroot_fsolver *s = gsl_multiroot_fsolver_alloc(T, this->equs.size());
    gsl_multiroot_function f = { &System::rosenbrock_f, (size_t)this->equs.size(), this };
    
    gsl_vector *x = gsl_vector_alloc(this->equs.size());
    for (int i = 0; i < this->equs.size(); i++)
        gsl_vector_set(x, i, 0.0); // 3rd arg is initial guess

    gsl_multiroot_fsolver_set(s, &f, x);

    int status = GSL_CONTINUE;
    size_t iter = 0;

    if (DEBUG_MODE) print_state(iter, this->equs.size(), s);
    while (status == GSL_CONTINUE && iter++ < 1000)
    {
        status = gsl_multiroot_fsolver_iterate(s);
        if (DEBUG_MODE) print_state(iter, this->equs.size(), s);
        if (status) /* check if solver is stuck */
            break;
        status = gsl_multiroot_test_residual(s->f, 1e-7);
    }

    debug("[%d] status = %s\n", this->sys_num, gsl_strerror(status));

    for (int i = 0; i < this->equs.size(); i++)
        res[i] = gsl_vector_get(s->x, i);

    gsl_multiroot_fsolver_free(s);
    gsl_vector_free(x);
    return 0;
}

int System::save_to_file(const std::string &fname, double *res) const
{
	FILE *f = fopen(fname.c_str(), "w");
	if (f == NULL)
		return -1;

	for (int i = 0; i < this->vars.size(); ++i)
		fprintf(f, "%s = %f\n", this->vars[i].c_str(), res[i]);

	fclose(f);
	return 0;
}

int System::save_to_markdown(const std::string &fname, double *res) const
{
	FILE *f = fopen(fname.c_str(), "w");
	if (f == NULL)
		return -1;

	fprintf(f, "# %s\n\n", fname.c_str());
	fprintf(f, "## System of equations\n\n");
	for (int i = 0; i < this->equs.size(); ++i)
	{
		fprintf(f, "$$");
		this->equs[i]->to_latex(f, *this);
		fprintf(f, "$$\n\n");
	}

	fprintf(f, "## Solution\n\n");
	for (int i = 0; i < this->vars.size(); ++i)
	{
		fprintf(f, "$$");
		Latex::var_to_latex(f, this->vars[i].c_str());
		fprintf(f, " = ");
		Latex::double_to_latex(f, res[i]);
		fprintf(f, "$$\n\n");
	}

	fclose(f);
	return 0;
}

void System::load_vars_from_exp(Expression *exp)
{
	switch (exp->type)
	{
	case EXPR_TYPE_DOUBLE:
		break;
	case EXPR_TYPE_VAR:
		this->add_var(exp->var);
		break;
	case EXPR_TYPE_ADD:
	case EXPR_TYPE_SUB:
	case EXPR_TYPE_MUL:
	case EXPR_TYPE_DIV:
	case EXPR_TYPE_EXP:
	case EXPR_TYPE_EQU:
		this->load_vars_from_exp(exp->eleft);
		this->load_vars_from_exp(exp->eright);
		break;
	case EXPR_TYPE_PAR:
		this->load_vars_from_exp(exp->eleft);
		break;
	}
}

void System::load_vars_from_equs()
{
	this->vars.clear();

	for (int i = 0; i < this->equs.size(); ++i)
	{
		this->load_vars_from_exp(this->equs[i]);
	}
}
