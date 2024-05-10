
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

void System::print() const
{
	std::cout << "System:" << std::endl;
	std::cout << "  Equations(" << this->equs.size() << "):" << std::endl;
	for (int i = 0; i < this->equs.size(); ++i)
	{
		std::cout << "    ";
		this->equs[i]->print();
		std::cout << std::endl;
	}
	std::cout << "  Variables(" << this->vars.size() << "):" << std::endl;
	for (auto &v : this->vars)
		std::cout << "    " << v.first << "{" /* << v.guess */ << "} [" << v.second->unit.to_string() << "]" << std::endl;
}

int System::rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f)
{
	System *sys = static_cast<System *>(params);

	std::vector<double> y(sys->equs.size());

	for (int i = 0; i < sys->equs.size(); ++i)
		y[i] = sys->equs[i]->eval(sys, x);

	if (x->size != sys->equs.size())
		std::cerr << "Error: x size is " << x->size << ", but system size is " << sys->equs.size() << std::endl, exit(1);

	for (int i = 0; i < sys->equs.size(); ++i)
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

int System::solve(std::vector<double> &res, std::vector<double> &guesses)
{
	const double EPSILON = 1e-7;
	const int MAX_ITERATIONS = 1000;
	const gsl_multiroot_fsolver_type *T = gsl_multiroot_fsolver_hybrids;
	gsl_multiroot_fsolver *s = gsl_multiroot_fsolver_alloc(T, this->equs.size());
	gsl_multiroot_function f = { &System::rosenbrock_f, (size_t)this->equs.size(), this };

	if (guesses.size() != this->equs.size())
		std::cerr << "Error: guesses size is " << guesses.size() << ", but system size is " << this->equs.size() << std::endl, exit(1);

	gsl_vector *x = gsl_vector_alloc(this->equs.size());
	for (int i = 0; i < this->equs.size(); i++)
		gsl_vector_set(x, i, guesses[i]);

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

	if (s->x->size != this->equs.size())
		std::cerr << "Error 2: x size is " << s->x->size << ", but system size is " << this->equs.size() << std::endl, exit(1);

	res = std::vector<double>(this->equs.size());
	for (int i = 0; i < this->equs.size(); i++)
		res[i] = gsl_vector_get(s->x, i);

	gsl_multiroot_fsolver_free(s);
	gsl_vector_free(x);
	return 0;
}

void System::infer_units()
{
	int it = 0;
	int not_stable = 1;

	while (not_stable)
	{
		not_stable = 0;
		for (int i = 0; i < this->equs.size(); ++i)
		{
			std::vector<ExpVar *> vars = std::vector<ExpVar *>();

			if (!this->equs[i]->infer_units(vars, SIUnit()))
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
		++it;
	}
	
	std::cout << "Infered units in " << it << " iterations" << std::endl;
}

System *System::deep_copy() const
{
	System *cp_sys = new System();

	for (int i = 0; i < this->equs.size(); ++i)
		cp_sys->add_equ(this->equs[i]->deep_copy());

	cp_sys->infer_units();

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

	int i = mother_sys->vars[this->name]->index;
	if (i == -1)
		std::cerr << "Error: variable " << this->name << " not found" << std::endl, exit(1);

	return gsl_vector_get(x, i);
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

	this->sys->infer_units();
}

double ExpFuncCall::eval(System *mother_sys, const gsl_vector *x) const
{
	funcs[this->f->name]->been_called = true;

	std::vector<double> args_values(this->args.size());
	for (int i = 0; i < this->args.size(); ++i)
		args_values[i] = this->args[i]->eval(mother_sys, x);

	System *cp_sys = this->sys->deep_copy();

	int j = 0;
	for (int i = 0; i < cp_sys->equs.size(); ++i)
	{
		ExpOp *equ = dynamic_cast<ExpOp *>(cp_sys->equs[i]);
		if (equ == nullptr)
			std::cerr << "Error: expression is not an equation" << std::endl, exit(1);

		ExpVar *var = dynamic_cast<ExpVar *>(equ->get_left());
		if (var != nullptr && var->name.front() == '@') // arguments of the function
		{
			cp_sys->add_equ(
				new ExpOp(
					OpType::EQU,
					new ExpVar(var->name.erase(0, 1)),
					new ExpNum(args_values[args_values.size() - 1 - i])));
			++j;
		}
	}

	for (int i = 0; i < j; ++i)
	{
		delete cp_sys->equs.front();
		cp_sys->equs.erase(cp_sys->equs.begin());
	}

	cp_sys->infer_units();

	std::vector<double> res;
	std::vector<double> guesses = std::vector<double>(cp_sys->equs.size(), 1.);

	cp_sys->solve(res, guesses);

	int i = cp_sys->vars["#ret"]->index;
	if (i == -1)
		std::cerr << "Error: variable #ret not found" << std::endl, exit(1);

	delete cp_sys;

	return res[i];
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

void ExpFuncCall::print() const
{
	std::cout << this->f->name << "(";
	this->sys->print();
	std::cout << ")";
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
	std::cerr << "Function: " << this->name << std::endl;
	std::cerr << "Args: ";
	for (int i = 0; i < this->args_names.size(); ++i)
		std::cerr << this->args_names[i] << " ";
	std::cerr << std::endl;
	std::cerr << "Fn system: " << std::endl;
	this->sys->print();
	std::cerr << "Expression: " << std::endl;
	this->exp->print();
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
