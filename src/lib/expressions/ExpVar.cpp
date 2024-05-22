
#include "ExpVar.hpp"

ExpVar::ExpVar(std::string name, std::string guess)
	: Exp(), name(name), guess(std::stod(guess.substr(1, guess.size() - 2))), can_be_infered(false), index(-1)
{

}

ExpVar::~ExpVar()
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

bool ExpVar::is_linear() const
{
    return true;
}

bool ExpVar::infer_units(std::vector<ExpVar *> &vars, SIUnit unit, bool is_value_known, double value)
{
	if (this->is_value_known && is_value_known)
		assert(abs(this->value - value) < 1e-6);
	if (this->unit.is_known && unit.is_known)
		assert(this->unit == unit);

	bool is_stable = true;

	if (!this->is_value_known && is_value_known)
	{
		this->is_value_known = is_value_known;
		this->value = value;
		this->can_be_infered = true;
		is_stable = false;
	}

	if (!this->unit.is_known && unit.is_known)
	{
		this->unit = unit;
		is_stable = false;
	}

	vars.push_back(this);
	return is_stable;
}

Exp *ExpVar::singularize_vars(System *sys)
{
	for (auto &v : sys->vars)
	{
		if (this == sys->vars[v.first])
			return this;
		if (this->name == v.first)
		{
			sys->vars_to_delete.push_back(this);
			return sys->vars[v.first];
		}
	}

	this->index = sys->vars.size();
	sys->vars[this->name] = this;
	return NULL;
}

bool ExpVar::is_completly_infered() const
{
	return this->is_value_known;
}

std::ostream &ExpVar::output(std::ostream &os) const
{
	os << this->name << "[" << this << "|" << this->value << "|" << this->unit << "]";
	return os;
}

void ExpVar::add_equs_from_func_calls(System *sys)
{
	return ;
}

void ExpVar::add_prefix_to_vars(std::string prefix)
{
	this->name = prefix + this->name;
}
