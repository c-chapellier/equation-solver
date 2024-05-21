
#include "ExpNum.hpp"

ExpNum::ExpNum(double value)
	: Exp(value, "[\\]")
{

}

ExpNum::ExpNum(double value, std::string unit)
	: Exp(value, unit)
{

}

ExpNum::~ExpNum()
{

}

double ExpNum::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->value;
}

ExpNum *ExpNum::deep_copy() const
{
	ExpNum *cp = new ExpNum(this->value);
	cp->unit = this->unit;
	return cp;
}

std::string ExpNum::to_latex() const
{
	return Latex::double_to_latex(this->value) + siu_to_latex(this->unit);
}

bool ExpNum::is_linear() const
{
    return true;
}

bool ExpNum::infer_units(std::vector<ExpVar *> &vars, siu_t unit, bool is_value_known, double value)
{
	if (this->is_value_known && is_value_known)
		assert(abs(this->value - value) < 1e-6);
	if (this->unit.is_known && unit.is_known)
		assert(siu_compare(this->unit, unit));

	assert(!(!this->is_value_known && is_value_known));

	if (!this->unit.is_known && unit.is_known)
	{
		this->unit = unit;
		return false;
	}

	return true;
}

Exp *ExpNum::singularize_vars(System *sys)
{
	return NULL;
}

bool ExpNum::is_completly_infered() const
{
	return this->is_value_known;
}

std::ostream &ExpNum::output(std::ostream &os) const
{
	os << this->value << "[" << this->value << "|" << siu_to_latex(this->unit) << "]";
	return os;
}

void ExpNum::add_equs_from_func_calls(System *sys)
{
	return ;
}

void ExpNum::add_prefix_to_vars(std::string prefix)
{
	return ;
}
