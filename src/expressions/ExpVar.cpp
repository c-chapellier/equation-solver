
#include "ExpVar.hpp"

<<<<<<< HEAD
#include <utility>


ExpVar::ExpVar(std::string var) : Exp(), var(std::move(std::move(var)))
=======
ExpVar::ExpVar(std::string var) : Exp()
>>>>>>> parent of a08e814 (run static analyzer)
{
	this->var = var;
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
