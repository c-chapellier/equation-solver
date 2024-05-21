
#include "ExpFuncCall.hpp"

ExpFuncCall::ExpFuncCall()
	: Exp(), args(std::vector<Exp *>()), f(nullptr), ret(nullptr)
{
	
}

ExpFuncCall::ExpFuncCall(Function *f, std::vector<Exp *> &args)
    : Exp(), ret(nullptr)
{
	if (f->args_names.size() != args.size())
		std::cerr << "Error: function " << f->name << " takes " << f->args_names.size() << " arguments, but " << args.size() << " were given" << std::endl, exit(1);

	this->f = f;
	this->args = args;
}

ExpFuncCall::~ExpFuncCall()
{
	for (int i = 0; i < this->args.size(); ++i)
		delete this->args[i];
	delete this->ret;
}

double ExpFuncCall::eval(System *mother_sys, const gsl_vector *x) const
{
	return this->ret->eval(mother_sys, x);
}

ExpFuncCall *ExpFuncCall::deep_copy() const
{
	ExpFuncCall *cp = new ExpFuncCall();

	cp->f = this->f;

	cp->args = std::vector<Exp *>();
	for (int i = 0; i < this->args.size(); ++i)
		cp->args.push_back(this->args[i]->deep_copy());

	cp->ret = this->ret ? this->ret->deep_copy() : nullptr;

	return cp;
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
	os << *this->ret;
	return os;
}

void ExpFuncCall::add_arg(Exp *arg)
{
	this->args.push_back(arg);
}

bool ExpFuncCall::is_linear() const
{
    return this->ret->is_linear();
}

bool ExpFuncCall::infer_units(std::vector<ExpVar *> &vars, SIUnit unit, bool is_value_known, double value)
{
	if (this->unit.is_known && unit.is_known)
        assert(this->unit == unit);
    
    if (this->is_value_known && is_value_known)
        assert(abs(this->value - value) < 1e-6);
    
    bool is_stable = true, ret;

    if (!this->unit.is_known && unit.is_known)
    {
        if (!this->is_value_known && is_value_known)
        {
            this->value = value;
            this->is_value_known = true;
        }
        this->unit = unit;
        is_stable = false;
    }

    if (!this->is_value_known && this->ret->is_value_known)
    {
        this->value = this->ret->value;
        this->is_value_known = true;
        is_stable = false;
    }

    if (!this->unit.is_known && this->ret->unit.is_known)
    {
        this->unit = this->ret->unit;
        is_stable = true;
    }

    ret = this->ret->infer_units(vars, this->unit, this->is_value_known, this->value);
    return is_stable && ret;
}

Exp *ExpFuncCall::singularize_vars(System *sys)
{
	Exp *r = this->ret->singularize_vars(sys);
    if (r != NULL)
        this->ret = r;
	
    return NULL;
}

bool ExpFuncCall::is_completly_infered() const
{
    return this->ret->is_completly_infered();
}

void ExpFuncCall::add_equs_from_func_calls(System *sys)
{
    std::string func_call_prefix = this->f->name + ":" + std::to_string(this->f->call_count++) + ":";

	for (int i = 0; i < this->f->args_names.size(); ++i)
	{
		ExpOp *arg = dynamic_cast<ExpOp *>(args[i]);
		if (arg != nullptr && arg->op == OpType::EQU)
			std::cerr << "Error: argument: " << this->f->args_names[i] << ": cannot use equation as argument" << std::endl, exit(1);

		sys->add_equ(
			new ExpOp(
				OpType::EQU,
				new ExpVar(func_call_prefix + this->f->args_names[i]),
				args[args.size() - 1 - i]->deep_copy()
			)
		);
	}

    this->ret = this->f->ret->deep_copy();
    this->ret->add_prefix_to_vars(func_call_prefix);

    System *tmp_sys = this->f->sys->deep_copy();
	for (int i = 0; i < this->f->sys->size(); ++i)
        tmp_sys->add_prefix_to_vars(func_call_prefix);
    sys->add_sys(tmp_sys);
    delete tmp_sys;
}

void ExpFuncCall::add_prefix_to_vars(std::string prefix)
{
    std::cerr << "ExpFuncCall::add_prefix_to_vars: not implemented" << std::endl, exit(1);
}
