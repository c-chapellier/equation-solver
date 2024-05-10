
#include "ExpNum.hpp"

ExpNum::ExpNum(double value)
	: Exp(value, "[\\]")
{

}

ExpNum::ExpNum(double value, std::string unit)
	: Exp(value, unit)
{

}

double ExpNum::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->value;
}

ExpNum *ExpNum::deep_copy() const
{
	if (this->unit.is_known)
		return new ExpNum(this->value, "[" + this->unit.to_string() + "]");
	return new ExpNum(this->value);
}

std::string ExpNum::to_latex() const
{
	if (this->unit.to_string() == "\\")
		return Latex::double_to_latex(this->value);
	return Latex::double_to_latex(this->value) + "\\,\\text{" + this->unit.to_string() + "}";
}

void ExpNum::print() const
{
	std::cout << this->value;
	std::cout << "[" << this->value << "|" << this->unit.to_string() << "]";
}

bool ExpNum::is_linear() const
{
    return true;
}

bool ExpNum::infer_units(std::vector<ExpVar *> &vars, SIUnit unit, bool is_value_known, double value)
{
	if (this->is_value_known && is_value_known && abs(this->value - value) > 1e-6)
	{
		std::cerr << "diff = " << abs(this->value - value) << std::endl;
		std::cerr << "this->value: " << this->value << std::endl;
		std::cerr << "value: " << value << std::endl;
		std::cerr << "Error: value mismatch: ExpNum::infer_units" << std::endl, exit(1);
	}
	if (this->unit.is_known && unit.is_known && this->unit.units != unit.units)
		std::cerr << "Error: unit mismatch: ExpNum::units_descent" << std::endl, exit(1);

	bool is_stable = true;

	if (!this->is_value_known && is_value_known)
	{
		std::cerr << "ExpNum::infer_units: should never be called" << std::endl, exit(1);
		this->is_value_known = is_value_known;
		this->value = value;
		is_stable = false;
	}

	if (!this->unit.is_known && unit.is_known)
	{
		this->unit = unit;
		is_stable = false;
	}

	return is_stable;
}

Exp *ExpNum::singularize_vars()
{
	return NULL;
}

bool ExpNum::is_completly_infered() const
{
	return this->is_value_known;
}
