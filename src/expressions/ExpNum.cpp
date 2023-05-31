
#include "ExpNum.hpp"

ExpNum::ExpNum(double dval) : Exp()
{
	this->dval = dval;
}

double ExpNum::eval(System *mother_sys, const gsl_vector *x)
{
    return this->dval;
}

ExpNum *ExpNum::deep_copy()
{
	return new ExpNum(this->dval);
}

void ExpNum::to_latex(FILE *f)
{
	Latex::double_to_latex(f, this->dval);
}

void ExpNum::print()
{
	printf("%f", this->dval);
}
