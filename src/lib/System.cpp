
#include "System.hpp"

System::System()
{
	this->equs = std::vector<ExpOp *>();
}

System::~System()
{
	for (int i = 0; i < this->equs.size(); ++i)
		delete this->equs[i];
	for (auto &v : this->vars)
		delete v.second;
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
			this->unknown_vars[var.first] = var.second;
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
	int not_stable = 1;

	while (not_stable)
	{
		not_stable = 0;
		for (int i = 0; i < this->equs.size(); ++i)
		{
			std::vector<ExpVar *> vars = std::vector<ExpVar *>();

			if (!this->equs[i]->infer_units(vars, SIUnit(), false))
				not_stable = 1;

			for (auto &v : this->vars)
			{
				if (!v.second->unit.is_known)
					not_stable = 1;
				if (this->vars[v.second->name]->can_be_infered)
					vars.erase(std::remove(vars.begin(), vars.end(), v.second), vars.end());
			}

			if (vars.size() == 1 && this->equs[i]->is_linear())
			{
				this->vars[vars[0]->name]->can_be_infered = true;
				not_stable = 1;
			}
		}
	}

	this->sort_equs_and_vars();
}

System *System::deep_copy() const
{
	System *cp_sys = new System();

	for (int i = 0; i < this->equs.size(); ++i)
		cp_sys->add_equ(this->equs[i]->deep_copy());

	cp_sys->infer();

	return cp_sys;
}

void System::singularize_vars()
{
	for (int i = 0; i < this->equs.size(); ++i)
		this->equs[i]->singularize_vars();
}

double ExpVar::eval(System *mother_sys, const gsl_vector *x) const
{
	if (this->name == "pi") return M_PI;
	if (this->name == "e") return M_E;

	int i = 0;
	for (auto &v : mother_sys->unknown_vars)
	{
		if (v.first == this->name)
			return gsl_vector_get(x, i);
		++i;
	}
	
	std::cerr << "Error: variable " << this->name << " not found" << std::endl, exit(1);
}

ExpFuncCall::ExpFuncCall(Function *f, std::vector<Exp *> &args) : Exp()
{
	if (f->args_names.size() != args.size())
		std::cerr << "Error: function " << f->name << " takes " << f->args_names.size() << " arguments, but " << args.size() << " were given" << std::endl, exit(1);

	this->sys = new System();
	this->f = f;
	this->args = args;

	for (int i = 0; i < this->f->args_names.size(); ++i)
	{
		if (dynamic_cast<ExpOp *>(args[i]) != nullptr)
			std::cerr << "Error: argument " << this->f->args_names[i] << " is an equation" << std::endl, exit(1);

		this->sys->add_equ(
			new ExpOp(
				OpType::EQU,
				new ExpVar(std::string("@") + this->f->args_names[i]),
				args[args.size() - 1 - i]->deep_copy()));
	}

	for (int i = 0; i < this->f->sys->size(); ++i)
		this->sys->add_equ(f->sys->equs[i]->deep_copy());

	this->sys->add_equ(
		new ExpOp(
			OpType::EQU,
			new ExpVar(std::string("#ret")),
			this->f->exp->deep_copy()));

	this->sys->infer();
}

double ExpFuncCall::eval(System *mother_sys, const gsl_vector *x) const
{
	// funcs[this->f->name]->been_called = true;

	// std::vector<double> args_values(this->args.size());
	// for (int i = 0; i < this->args.size(); ++i)
	// 	args_values[i] = this->args[i]->eval(mother_sys, x);

	// System *cp_sys = this->sys->deep_copy();

	// int j = 0;
	// for (int i = 0; i < cp_sys->equs.size(); ++i)
	// {
	// 	ExpOp *equ = dynamic_cast<ExpOp *>(cp_sys->equs[i]);
	// 	if (equ == nullptr)
	// 		std::cerr << "Error: expression is not an equation" << std::endl, exit(1);

	// 	ExpVar *var = dynamic_cast<ExpVar *>(equ->get_left());
	// 	if (var != nullptr && var->name.front() == '@') // arguments of the function
	// 	{
	// 		cp_sys->add_equ(
	// 			new ExpOp(
	// 				OpType::EQU,
	// 				new ExpVar(var->name.erase(0, 1)),
	// 				new ExpNum(args_values[args_values.size() - 1 - i])));
	// 		++j;
	// 	}
	// }

	// for (int i = 0; i < j; ++i)
	// {
	// 	delete cp_sys->equs.front();
	// 	cp_sys->equs.erase(cp_sys->equs.begin());
	// }

	// cp_sys->infer_units();

	// std::vector<double> res;
	// std::vector<double> guesses = std::vector<double>(cp_sys->equs.size(), 1.);

	// cp_sys->solve(res, guesses);

	// int i = cp_sys->vars["#ret"]->index;
	// assert(i != -1)

	// delete cp_sys;

	// return res[i];
}

ExpFuncCall *ExpFuncCall::deep_copy() const
{
	std::vector<Exp *> cp_args = std::vector<Exp *>();

	for (int i = 0; i < this->args.size(); ++i)
		cp_args.push_back(this->args[i]->deep_copy());

	ExpFuncCall *ret = new ExpFuncCall();

	ret->f = this->f->deep_copy();
	ret->args = cp_args;
	ret->sys = this->sys->deep_copy();

	return ret;
}

std::string ExpFuncCall::to_latex() const
{
	if (this->f->name == "abs")
		return "\\mid " + this->args[0]->to_latex() + " \\mid";
	else if (this->f->name == "sqrt")
		return "\\sqrt{" + this->args[0]->to_latex() + "}";
	else
	{
		std::string ret = Latex::var_to_latex(this->f->name);

		ret += "\\left(\n";
		for (int i = 0; i < this->args.size(); ++i)
		{
			ret += this->args[i]->to_latex();
			if (i != this->args.size() - 1)
				ret += ", ";
		}
		ret += "\\right)";
		return ret;
	}
}

std::ostream &ExpFuncCall::output(std::ostream &os) const
{
	os << this->f->name << "(";
	os << this->sys;
	os << ")";
	return os;
}

Function *Function::deep_copy() const
{
	std::vector<std::string> cp_args_names = std::vector<std::string>();
	for (int i = 0; i < this->args_names.size(); ++i)
		cp_args_names.push_back(this->args_names[i]);

	return new Function(this->name, cp_args_names, this->sys->deep_copy(), this->exp->deep_copy());
}

void Function::print() const
{
	std::cout << "Function: " << this->name << std::endl;
	std::cout << "Args: ";
	for (int i = 0; i < this->args_names.size(); ++i)
		std::cout << this->args_names[i] << " ";
	std::cout << std::endl;
	std::cout << "Fn system: " << std::endl;
	std::cout << this->sys;
	std::cout << "Expression: " << std::endl;
	std::cout << this->exp << std::endl;
}

std::string Function::to_latex() const
{
	std::string res = "";
	res += "" + Latex::var_to_latex(this->name) + "(";
	for (int i = 0; i < this->args_names.size(); ++i)
	{
		res += this->args_names.at(i);
		if (i != this->args_names.size() - 1)
			res += ", ";
	}
	res += ")";
	if (this->sys->equs.size() > 0)
		res += ":";
	for (int i = 0; i < this->sys->equs.size(); ++i)
		res += this->sys->equs[i]->to_latex() + " ; ";
	res += " \\rArr " + this->exp->to_latex() + "";
	return res;
}

Function::~Function()
{
	delete this->sys;
	delete this->exp;
}

ExpFuncCall::~ExpFuncCall()
{
	delete this->sys;
	delete this->f;
	for (int i = 0; i < this->args.size(); ++i)
		delete this->args[i];
}
