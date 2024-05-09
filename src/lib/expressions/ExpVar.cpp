
#include "ExpVar.hpp"

ExpVar::ExpVar(std::string var, std::string guess)
	: Exp(), var(var), guess(std::stod(guess.substr(1, guess.size() - 2))), true_var(true), index(-1)
{

}

ExpVar *ExpVar::deep_copy() const
{
	return new ExpVar(this->var, "{" + std::to_string(this->guess) + "}");
}

std::string ExpVar::to_latex() const
{
	return Latex::var_to_latex(this->var.c_str());
}

void ExpVar::print() const
{
	std::cout << this->var;
	debug_units && std::cout << "[" << this->unit.to_string() << "]";
}

bool ExpVar::is_linear() const
{
    return true;
}

std::vector<ExpVar *> ExpVar::get_vars()
{
	return std::vector<ExpVar *>({this});
}

void ExpVar::units_descent(SIUnit unit)
{
	if (this->unit.unit_known && this->unit.units != unit.units)
		std::cerr << "Error: unit mismatch: ExpVar::units_descent" << std::endl, exit(1);

	if (!this->unit.unit_known)
		this->unit = unit;
}

Exp *ExpVar::singularize_vars()
{
	for (auto &v : main_sys.singularized_vars_map)
		if (this->var == v.first)
			return main_sys.singularized_vars_map[v.first];

	this->index = main_sys.singularized_vars_map.size();
	main_sys.singularized_vars_map[this->var] = this;
	return NULL;
}
