
#include "ExpNum.hpp"

ExpNum::ExpNum(double dval)
	: Exp("[\\]"), dval(dval)
{

}

ExpNum::ExpNum(double dval, std::string unit)
	: Exp(unit), dval(dval)
{

}

double ExpNum::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->dval;
}

ExpNum *ExpNum::deep_copy() const
{
	if (this->unit.is_known)
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
	debug_units && std::cout << "[" << this->unit.is_known << ", " << this->unit.to_string() << "]";
}

bool ExpNum::is_linear() const
{
    return true;
}

bool ExpNum::infer_units(std::vector<ExpVar *> &vars, SIUnit unit)
{
	if (this->unit.is_known && unit.is_known && this->unit.units != unit.units)
		std::cerr << "Error: unit mismatch: ExpNum::units_ascent" << std::endl, exit(1);

	if (!this->unit.is_known && unit.is_known)
	{
		this->unit = unit;
		return false;
	}

	return true;
}

Exp *ExpNum::singularize_vars()
{
	return NULL;
}
