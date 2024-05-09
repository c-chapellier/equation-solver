
#include "ExpNum.hpp"

ExpNum::ExpNum(double dval)
	: Exp(), dval(dval)
{

}

ExpNum::ExpNum(double dval, std::string unit)
	: Exp(unit), dval(dval)
{

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
	if (this->unit.unit_known)
		return new ExpNum(this->dval, "[" + this->unit.to_string() + "]");
	return new ExpNum(this->dval);
}

std::string ExpNum::to_latex() const
{
	if (this->unit.to_string() == "\\")
		return Latex::double_to_latex(this->dval);
	return Latex::double_to_latex(this->dval) + "\\,\\text{" + this->unit.to_string() + "}";
}

void ExpNum::print() const
{
	std::cout << this->dval;
	debug_units && std::cout << "[" << this->unit.unit_known << ", " << this->unit.to_string() << "]";
}

bool ExpNum::is_linear() const
{
    return true;
}

std::vector<ExpVar *> ExpNum::get_vars()
{
	// units ok (in constructor)
	return std::vector<ExpVar *>();
}

void ExpNum::units_descent(SIUnit unit)
{
	if (this->unit.units != unit.units)
	{
		std::cerr << "Error: unit mismatch: ExpNum::units_descent" << std::endl;
		exit(1);
	}
}

Exp *ExpNum::singularize_vars()
{
	return NULL;
}
