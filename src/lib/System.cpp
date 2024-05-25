
#include "System.hpp"

System::System()
{
	this->equs = std::vector<ExpOp *>();
}

System::~System()
{
	this->singularize_vars();

	for (int i = 0; i < this->equs.size(); ++i)
		delete this->equs[i];

	for (auto &v : this->vars)
		delete v.second;

	for (auto &v : this->vars_to_delete)
		delete v;
}

size_t System::size() const
{
	return this->equs.size();
}

void System::add_equ(ExpOp *equ)
{
	this->equs.push_back(equ);
}

void System::add_sys(System *sys)
{
	for (int i = 0; i < sys->equs.size(); ++i)
		this->add_equ(sys->equs[i]->deep_copy());
}

void System::add_equs_from_func_calls()
{
	for (int i = 0; i < this->equs.size(); ++i)
		this->equs[i]->add_equs_from_func_calls(this);
}

std::ostream &operator<<(std::ostream &os, const System &sys)
{
    os << "System:" << std::endl;
	os << "  Equations(" << sys.equs.size() << "):" << std::endl;
	for (int i = 0; i < sys.equs.size(); ++i)
		os << "    " << *sys.equs[i] << std::endl;
	os << "  Variables(" << sys.vars.size() << "):" << std::endl;
	for (auto &v : sys.vars)
		os << "    " << v.first << "{" << v.second->guess << "} [" << v.second->unit << "]" << std::endl;
    return os;
}

int System::rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f)
{
	System *sys = static_cast<System *>(params);

	std::vector<double> y(sys->unknown_equs.size());

	for (int i = 0; i < sys->unknown_equs.size(); ++i)
		y[i] = sys->unknown_equs[i]->eval(sys, x);

	assert(x->size == sys->unknown_equs.size());

	for (int i = 0; i < sys->unknown_equs.size(); ++i)
		gsl_vector_set(f, i, y[i]);

	return GSL_SUCCESS;
}

void System::print_state(size_t iter, int n, gsl_multiroot_fsolver *s)
{
	std::cout << "iter = " << iter << " x = ";
	for (int i = 0; i < n; ++i)
		std::cout << gsl_vector_get(s->x, i) << " ";
	std::cout << "f(x) = ";
	for (int i = 0; i < n; ++i)
		std::cout << gsl_vector_get(s->f, i) << " ";
	std::cout << std::endl;
}

int System::solve()
{
	int n = this->unknown_equs.size();

	if (n == 0)
		return 0;

	const double EPSILON = 1e-7;
	const int MAX_ITERATIONS = 1000;
	const gsl_multiroot_fsolver_type *T = gsl_multiroot_fsolver_hybrids;
	gsl_multiroot_fsolver *s = gsl_multiroot_fsolver_alloc(T, n);
	gsl_multiroot_function f = { &System::rosenbrock_f, (size_t)n, this };
	gsl_vector *x = gsl_vector_alloc(n);
	
	int i = 0;
	for (auto &v : this->unknown_vars)
		gsl_vector_set(x, i++, v.second->guess);

	gsl_multiroot_fsolver_set(s, &f, x);

	int status = GSL_CONTINUE;
	size_t iter = 0;

	while (status == GSL_CONTINUE && iter++ < MAX_ITERATIONS)
	{
		status = gsl_multiroot_fsolver_iterate(s);
		if (status) /* check if solver is stuck */
		{
			std::cerr << "Error 1: solver is stuck" << std::endl;
			break;
		}
		status = gsl_multiroot_test_residual(s->f, EPSILON);
	}

	assert(s->x->size == this->unknown_equs.size());

	i = 0;
	for (auto &v : this->unknown_vars)
	{
		v.second->value = gsl_vector_get(s->x, i++);
		v.second->is_value_known = true;
	}

	gsl_multiroot_fsolver_free(s);
	gsl_vector_free(x);
	return 0;
}

void System::sort_equs_and_vars()
{
	for (auto &var : this->vars)
	{
		if (var.second->is_value_known)
			this->inferred_vars[var.first] = var.second;
		else
		{
			var.second->state = ExpVar::UNKNOWN;
			this->unknown_vars[var.first] = var.second;
		}
	}

	for (int i = 0; i < this->equs.size(); ++i)
	{
		if (this->equs[i]->is_completly_infered())
			this->inferred_equs.push_back(this->equs[i]);
		else
			this->unknown_equs.push_back(this->equs[i]);
	}

	assert(this->unknown_equs.size() == this->unknown_vars.size());
}

void System::infer()
{
	bool stable = false, first_time_stable = true;

	while (!stable)
	{
		std::cout << *this;
		stable = true;
		for (int i = 0; i < this->equs.size(); ++i)
		{
			std::vector<ExpVar *> equs_vars = std::vector<ExpVar *>();

			if (!this->equs[i]->infer_units(equs_vars, SIUnit(), false))
				stable = false;

			for (int j = 0; j < equs_vars.size(); ++j)
				if (equs_vars[j]->is_value_known || equs_vars[j]->unit.is_known || equs_vars[j]->state == ExpVar::CONSTANT || equs_vars[j]->state == ExpVar::INFERED)
					equs_vars.erase(equs_vars.begin() + j--);

			if (equs_vars.size() == 1 && this->equs[i]->is_linear())
			{
				this->vars[equs_vars[0]->name]->state = ExpVar::INFERED;
				stable = false;
			}
		}

		if (stable && first_time_stable)
		{
			stable = false;
			first_time_stable = false;
		}
	}

	this->sort_equs_and_vars();
}

System *System::deep_copy() const
{
	System *cp_sys = new System();

	for (int i = 0; i < this->equs.size(); ++i)
		cp_sys->add_equ(this->equs[i]->deep_copy());

	return cp_sys;
}

void System::singularize_vars()
{
	for (int i = 0; i < this->equs.size(); ++i)
		this->equs[i]->singularize_vars(this);
}

void System::add_prefix_to_vars(std::string prefix)
{
	for (int i = 0; i < this->equs.size(); ++i)
		this->equs[i]->add_prefix_to_vars(prefix);
}

double ExpVar::eval(System *mother_sys, const gsl_vector *x) const
{
	int i = 0;
	for (auto &v : mother_sys->unknown_vars)
	{
		if (v.first == this->name)
			return gsl_vector_get(x, i);
		++i;
	}

	for (auto &v : mother_sys->inferred_vars)
	{
		if (v.first == this->name)
			return v.second->value;
	}

	std::cerr << "Error: variable " << this->name << " not found" << std::endl, exit(1);
}
