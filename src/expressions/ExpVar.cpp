
#include "ExpVar.hpp"

ExpVar::ExpVar(std::string var) : Exp(), var(var)
{
	
}

auto ExpVar::deep_copy() const -> ExpVar *
{
	return new ExpVar(this->var);
}

auto ExpVar::to_latex() const -> std::string
{
	return Latex::var_to_latex(this->var);
}

void ExpVar::print() const
{
	std::cout << this->var;
}
