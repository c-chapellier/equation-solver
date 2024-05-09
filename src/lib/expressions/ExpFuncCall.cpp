
#include "ExpFuncCall.hpp"

ExpFuncCall::ExpFuncCall()
	: Exp(), sys(nullptr), args(std::vector<Exp *>()), f(nullptr)
{
	
}

void ExpFuncCall::load_vars_into_sys(System *sys) const
{
	for (int i = 0; i < this->args.size(); ++i)
		this->args[i]->load_vars_into_sys(sys);
}

void ExpFuncCall::add_arg(Exp *arg)
{
	this->args.push_back(arg);
}

bool ExpFuncCall::is_linear() const
{
    return false; // TODO
}

std::vector<ExpVar *> ExpFuncCall::get_vars()
{
	std::cerr << "Not implemented: ExpFuncCall::get_vars()" << std::endl, exit(1);	// TODO
}

void ExpFuncCall::units_descent(SIUnit unit)
{
	std::cerr << "Not implemented: ExpFuncCall::units_descent()" << std::endl, exit(1);	// TODO
}

Exp *ExpFuncCall::singularize_vars()
{
    std::cerr << "ExpFuncCall::singularize_vars: not implemented" << std::endl, exit(1);
}
