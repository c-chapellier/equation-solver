
#include "ExpNum.hpp"

ExpNum::ExpNum(double dval) : Exp()
{
	this->dval = dval;
}

void ExpNum::load_vars_into_sys(System *sys) const
{

}

double ExpNum::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->dval;
}

ExpNum *ExpNum::deep_copy() const
{
	return new ExpNum(this->dval);
}

std::string ExpNum::to_latex() const
{
	return Latex::double_to_latex(this->dval);
}

void ExpNum::print() const
{
	std::cout << this->dval;
}
