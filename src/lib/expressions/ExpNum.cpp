
#include "ExpNum.hpp"

ExpNum::ExpNum(double dval, std::string unit)
	: Exp(), dval(dval), unit(unit)
{
	std::cout << "ExpNum(" << this->dval << ", " << this->unit << ")" << std::endl;
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
	return new ExpNum(this->dval, this->unit);
}

std::string ExpNum::to_latex() const
{
	if (this->unit == "")
		return Latex::double_to_latex(this->dval);
	return Latex::double_to_latex(this->dval) + "\\,\\text{" + this->unit + "}";
}

void ExpNum::print() const
{
	std::cout << this->dval;
}
