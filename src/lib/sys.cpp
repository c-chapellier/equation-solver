
#include "es.hpp"

void sys_free(system_t *sys)
{
	sys_singularize_vars(sys);

	for (int i = 0; i < sys->equs.size(); ++i)
		delete sys->equs[i];

	for (auto &v : sys->vars)
		delete v.second;
}

size_t sys_size(system_t *sys)
{
	return sys->equs.size();
}

void sys_add_equ(system_t *sys, ExpOp *equ)
{
	sys->equs.push_back(equ);
}

void sys_add_sys(system_t *sys, system_t *sys_to_add)
{
	for (int i = 0; i < sys_to_add->equs.size(); ++i)
		sys_add_equ(sys, sys_to_add->equs[i]->deep_copy());
}

void sys_add_equs_from_func_calls(system_t *sys)
{
	for (int i = 0; i < sys->equs.size(); ++i)
		sys->equs[i]->add_equs_from_func_calls(sys);
}

int sys_rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f)
{
	system_t *sys = static_cast<system_t *>(params);

	std::vector<double> y(sys->unknown_equs.size());

	for (int i = 0; i < sys->unknown_equs.size(); ++i)
		y[i] = sys->unknown_equs[i]->eval(sys, x);

	assert(x->size == sys->unknown_equs.size());

	for (int i = 0; i < sys->unknown_equs.size(); ++i)
		gsl_vector_set(f, i, y[i]);

	return GSL_SUCCESS;
}

void sys_print_state(size_t iter, int n, gsl_multiroot_fsolver *s)
{
	std::cout << "iter = " << iter << " x = ";
	for (int i = 0; i < n; ++i)
		std::cout << gsl_vector_get(s->x, i) << " ";
	std::cout << "f(x) = ";
	for (int i = 0; i < n; ++i)
		std::cout << gsl_vector_get(s->f, i) << " ";
	std::cout << std::endl;
}

int sys_solve(system_t *sys)
{
	int n = sys->unknown_equs.size();
	const double EPSILON = 1e-7;
	const int MAX_ITERATIONS = 1000;
	const gsl_multiroot_fsolver_type *T = gsl_multiroot_fsolver_hybrids;
	gsl_multiroot_fsolver *s = gsl_multiroot_fsolver_alloc(T, n);
	gsl_multiroot_function f = { &sys_rosenbrock_f, (size_t)n, sys };
	gsl_vector *x = gsl_vector_alloc(n);
	
	int i = 0;
	for (auto &v : sys->unknown_vars)
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

	assert(s->x->size == sys->unknown_equs.size());

	i = 0;
	for (auto &v : sys->unknown_vars)
	{
		v.second->value = gsl_vector_get(s->x, i++);
		v.second->is_value_known = true;
	}

	gsl_multiroot_fsolver_free(s);
	gsl_vector_free(x);
	return 0;
}

void sys_sort_equs_and_vars(system_t *sys)
{
	for (auto &var : sys->vars)
	{
		if (var.second->is_value_known)
			sys->inferred_vars[var.first] = var.second;
		else
			sys->unknown_vars[var.first] = var.second;
	}

	for (int i = 0; i < sys->equs.size(); ++i)
	{
		if (sys->equs[i]->is_completly_infered())
			sys->inferred_equs.push_back(sys->equs[i]);
		else
			sys->unknown_equs.push_back(sys->equs[i]);
	}

	assert(sys->unknown_equs.size() == sys->unknown_vars.size());
}

void sys_infer(system_t *sys)
{
	int not_stable = 1;
	int it = 0;

	while (not_stable)
	{
		// DEBUG("Iteration: " << it++);
		not_stable = 0;
		for (int i = 0; i < sys->equs.size(); ++i)
		{
			// DEBUG(not_stable << " Equation(" << i << "): " << *this->equs[i]);
			std::vector<ExpVar *> vars = std::vector<ExpVar *>();

			siu_t unit;
			siu_init(&unit);

			if (!sys->equs[i]->infer_units(vars, unit, false))
				not_stable = 1;

			// DEBUG(not_stable << " Equation(" << i << "): " << *this->equs[i]);

			// DEBUG("vars: " << vars.size());
			// for (auto &v : vars)
			// 	DEBUG("  " << *v);

			for (int j = 0; j < vars.size(); ++j)
				if (vars[j]->is_value_known || vars[j]->unit.is_known || vars[j]->can_be_infered)
					vars.erase(vars.begin() + j--);

			// DEBUG("vars: " << vars.size());
			// for (auto &v : vars)
			// {
			// 	DEBUG("v.is_value_known = " << v->is_value_known);
			// 	DEBUG("v.unit.is_known = " << v->unit.is_known);
			// 	DEBUG("v.can_be_infered = " << v->can_be_infered);
			// 	DEBUG("  " << *v);
			// }

			// DEBUG("not_stable = " << not_stable);

			if (vars.size() == 1 && sys->equs[i]->is_linear())
			{
				sys->vars[vars[0]->name]->can_be_infered = true;
				not_stable = 1;
			}

			// DEBUG("not_stable = " << not_stable);
		}
	}

	sys_sort_equs_and_vars(sys);
}

system_t *sys_deep_copy(system_t *sys)
{
	system_t *cp_sys = new system_t();

	for (int i = 0; i < sys->equs.size(); ++i)
		sys_add_equ(cp_sys, sys->equs[i]->deep_copy());

	return cp_sys;
}

void sys_singularize_vars(system_t *sys)
{
	for (int i = 0; i < sys->equs.size(); ++i)
		sys->equs[i]->singularize_vars(sys);
}

void sys_add_prefix_to_vars(system_t *sys, std::string prefix)
{
	for (int i = 0; i < sys->equs.size(); ++i)
		sys->equs[i]->add_prefix_to_vars(prefix);
}

double ExpVar::eval(system_t *mother_sys, const gsl_vector *x) const
{
	// if (this->name == "pi") return M_PI;
	// if (this->name == "e") return M_E;


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
