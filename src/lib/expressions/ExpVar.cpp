
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
	debug_units && std::cout << "[" << this->unit.to_string() << "]";
}

bool ExpVar::is_linear() const
{
    return true;
}

bool ExpVar::infer_units(std::vector<ExpVar *> &vars, SIUnit unit)
{
	vars.push_back(this);

	if (this->unit.is_known && unit.is_known && this->unit.units != unit.units)
		std::cerr << "Error: unit mismatch: ExpVar::units_descent" << std::endl, exit(1);

	if (!this->unit.is_known && unit.is_known)
	{
		this->unit = unit;
		return false;
	}

	return true;
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
