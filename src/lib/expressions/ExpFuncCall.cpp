
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

std::vector<ExpVar *> ExpFuncCall::units_ascent()
{
	std::cerr << "Not implemented: ExpFuncCall::units_ascent()" << std::endl, exit(1);	// TODO
}

void ExpFuncCall::units_descent(SIUnit unit)
{
	std::cerr << "Not implemented: ExpFuncCall::units_descent()" << std::endl, exit(1);	// TODO
}

Exp *ExpFuncCall::singularize_vars()
{
    std::cerr << "ExpFuncCall::singularize_vars: not implemented" << std::endl, exit(1);
}
