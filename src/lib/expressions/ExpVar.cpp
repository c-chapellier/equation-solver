
#include "ExpVar.hpp"

ExpVar::ExpVar(std::string name, std::string guess)
	: Exp(), name(name), guess(std::stod(guess.substr(1, guess.size() - 2))), is_true_var(true), index(-1)
{

}

ExpVar *ExpVar::deep_copy() const
{
	return new ExpVar(this->name, "{" + std::to_string(this->guess) + "}");
}

std::string ExpVar::to_latex() const
{
	return Latex::var_to_latex(this->name.c_str());
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
	for (auto &v : main_sys.vars)
		if (this->name == v.first)
			return main_sys.vars[v.first];

	this->index = main_sys.vars.size();
	main_sys.vars[this->name] = this;
	return NULL;
}
