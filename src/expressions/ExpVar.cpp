
#include "ExpVar.hpp"

ExpVar::ExpVar(std::string var) : Exp()
{
	this->var = var;
}

ExpVar *ExpVar::deep_copy()
{
	return new ExpVar(this->var);
}

void ExpVar::to_latex(FILE *f)
{
	Latex::var_to_latex(f, this->var.c_str());
}

void ExpVar::print()
{
	printf("%s", this->var.c_str());
}
