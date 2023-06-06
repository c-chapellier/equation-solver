
#include "ExpVar.hpp"

ExpVar::ExpVar(std::string var)
	: Exp(), dval(0.), var(var)
{

}

ExpVar *ExpVar::deep_copy() const
{
	return new ExpVar(this->var);
}

std::string ExpVar::to_latex() const
{
	return Latex::var_to_latex(this->var.c_str());
}

void ExpVar::print() const
{
	std::cout << this->var;
}
