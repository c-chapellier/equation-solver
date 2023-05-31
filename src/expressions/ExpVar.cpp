
#include "ExpVar.hpp"

ExpVar::ExpVar(std::string var) : Exp()
{
	this->var = var;
}

ExpVar *ExpVar::deep_copy() const
{
	return new ExpVar(this->var);
}

void ExpVar::to_latex(std::ofstream &f) const
{
	Latex::var_to_latex(f, this->var.c_str());
}

void ExpVar::print() const
{
	std::cout << this->var;
}
