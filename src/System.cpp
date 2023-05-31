
#include "System.hpp"

System::System()
{
	this->equs = std::vector<Exp *>();
	this->vars = std::vector<std::string>();
}

int System::size() const
{
	return this->equs.size();
}

void System::add_equ(Exp *equ)
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
	printf("iter = %3lu x = ", iter);
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

    debug("status = %s\n", gsl_strerror(status));

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
		this->equs[i]->to_latex(f);
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

void System::load_vars_from_equs()
{
	this->vars.clear();

	for (int i = 0; i < this->equs.size(); ++i)
		this->equs[i]->load_vars_into_sys(this);
}

void ExpVar::load_vars_into_sys(System *sys)
{
	sys->add_var(this->var);
}

double ExpVar::eval(System *mother_sys, const gsl_vector *x)
{
	if (this->var == "")
		return this->dval;

	int var_index;
	for (int i = 0; i < mother_sys->vars.size(); ++i)
		if (mother_sys->vars[i] == this->var)
			var_index = i;

	if (var_index == -1)
		fprintf(stderr, "Error: variable %s not found\n", this->var.c_str()), exit(1);

    return gsl_vector_get(x, var_index);
}

double ExpFuncCall::eval(System *mother_sys, const gsl_vector *x)
{
	std::vector<double> args;
	for (int i = 0; i < this->args.size(); ++i)
		args.push_back(this->args[i]->eval(mother_sys, x));
	
	System *cp_sys = new System();

	for (int i = 0; i < this->sys->size(); ++i)
		cp_sys->add_equ(this->sys->equs[i]->deep_copy());
	cp_sys->load_vars_from_equs();

	int j = 0;
	for (int i = 0; i < cp_sys->equs.size(); ++i)
	{
		ExpEqu *equ = dynamic_cast<ExpEqu *>(cp_sys->equs[i]);
		if (equ == nullptr)
			fprintf(stderr, "Error: expression is not an equation\n"), exit(1);

		ExpVar *var = dynamic_cast<ExpVar *>(equ->eleft);
		if (var != nullptr && var->var.front() == '@')
		{
			std::string var = cp_sys->vars[i];
			var.erase(0, 1);
			cp_sys->add_equ(
				new ExpEqu(
					new ExpVar(var),
					new ExpNum(args[i])
				)
			);
			++j;
		}
	}

	for (int i = 0; i < j; ++i)
		cp_sys->equs.erase(cp_sys->equs.begin());

	cp_sys->load_vars_from_equs();

	double res[cp_sys->size()];
	cp_sys->solve(res);

	return res[cp_sys->size() - 1];
}

ExpFuncCall *ExpFuncCall::deep_copy()
{
	ExpFuncCall *exp = new ExpFuncCall(this->var, NULL);

    exp->sys = new System();
	for (int i = 0; i < this->sys->size(); ++i)
		exp->sys->add_equ(this->sys->equs[i]->deep_copy());
	
	return exp;
}

void ExpFuncCall::print()
{
	printf("%s(\n", this->var.c_str());
	this->sys->print();
	printf(")\n");
}
