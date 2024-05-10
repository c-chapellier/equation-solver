
#include "ExpVar.hpp"

ExpVar::ExpVar(std::string name, std::string guess)
	: Exp(), name(name), guess(std::stod(guess.substr(1, guess.size() - 2))), can_be_infered(false), index(-1)
{

}

ExpVar *ExpVar::deep_copy() const
{
	return new ExpVar(this->name, "{" + std::to_string(this->guess) + "}");
}

std::string ExpVar::to_latex() const
{
	if (!this->can_be_infered)
		return "\\textcolor{red}{" + Latex::var_to_latex(this->name.c_str()) + "}";
	return "\\textcolor{green}{" + Latex::var_to_latex(this->name.c_str()) + "}";
}

void ExpVar::print() const
{
	std::cout << this->name;
	std::cout << "[" << this->value << "|" << this->unit.to_string() << "]";
}

bool ExpVar::is_linear() const
{
    return true;
}

bool ExpVar::infer_units(std::vector<ExpVar *> &vars, SIUnit unit, bool is_value_known, double value)
{
	vars.push_back(this);

	if (this->is_value_known && is_value_known && abs(this->value - value) > 1e-6)
	{
		std::cerr << "diff = " << abs(this->value - value) << std::endl;
		std::cerr << "this->value: " << this->value << std::endl;
		std::cerr << "value: " << value << std::endl;
		std::cerr << "Error: value mismatch: ExpVar::infer_units" << std::endl, exit(1);
	}
	if (this->unit.is_known && unit.is_known && this->unit.units != unit.units)
		std::cerr << "Error: unit mismatch: ExpVar::units_descent" << std::endl, exit(1);

	bool is_stable = true;

	if (!this->is_value_known && is_value_known)
	{
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

Exp *ExpVar::singularize_vars()
{
	for (auto &v : main_sys.vars)
		if (this->name == v.first)
			return main_sys.vars[v.first];

	this->index = main_sys.vars.size();
	main_sys.vars[this->name] = this;
	return NULL;
}

bool ExpVar::is_completly_infered() const
{
	return this->is_value_known;
}
