
#include "ExpFuncCall.hpp"

ExpFuncCall::ExpFuncCall()
	: Exp(), sys(nullptr), args(std::vector<Exp *>()), f(nullptr)
{
	
}

void ExpFuncCall::add_arg(Exp *arg)
{
	this->args.push_back(arg);
}

bool ExpFuncCall::is_linear() const
{
    return false; // TODO
}

bool ExpFuncCall::infer_units(std::vector<ExpVar *> &vars, SIUnit unit)
{
	std::cerr << "Not implemented: ExpFuncCall::infer_units()" << std::endl, exit(1);	// TODO
}

Exp *ExpFuncCall::singularize_vars()
{
    std::cerr << "ExpFuncCall::singularize_vars: not implemented" << std::endl, exit(1);
}
