
#include "ExpNum.hpp"

ExpNum::ExpNum(double dval) : Exp(), dval(dval)
{
	
}

void ExpNum::load_vars_into_sys(System *sys) const
{

}

auto ExpNum::eval(System * /*mother_sys*/, const gsl_vector * /*x*/) const -> double
{
    return this->dval;
}

auto ExpNum::deep_copy() const -> ExpNum *
{
	return new ExpNum(this->dval);
}

auto ExpNum::to_latex() const -> std::string
{
	return Latex::double_to_latex(this->dval);
}

void ExpNum::print() const
{
	std::cout << this->dval;
}
