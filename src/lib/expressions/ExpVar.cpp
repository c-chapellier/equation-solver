
#include "ExpVar.hpp"

ExpVar::ExpVar(std::string name, std::string guess, State state)
	: Exp(), name(name), guess(std::stod(guess.substr(1, guess.size() - 2))), state(state)
{

}

ExpVar::~ExpVar()
{

}

ExpVar *ExpVar::deep_copy() const
{
	return new ExpVar(this->name, "{" + std::to_string(this->guess) + "}", this->state);
}

std::string ExpVar::to_latex() const
{
	switch (this->state)
	{
	case ExpVar::NORMAL:
		return Latex::var_to_latex(this->name.c_str());
	case ExpVar::CONSTANT:
		return "\\textcolor{yellow}{" + Latex::var_to_latex(this->name.c_str()) + "}";
	case ExpVar::INFERED:
		return "\\textcolor{green}{" + Latex::var_to_latex(this->name.c_str()) + "}";
	case ExpVar::UNKNOWN:
		return "\\textcolor{red}{" + Latex::var_to_latex(this->name.c_str()) + "}";
	}
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
		this->state = ExpVar::INFERED;
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
	std::string last_part = this->name.substr(this->name.find_last_of(":") + 1);
	if (sys->default_constants.find(last_part) != sys->default_constants.end())
	{
		this->is_value_known = true;
		this->value = sys->default_constants[last_part];
		this->unit = SIUnit("[\\]");
		this->state = ExpVar::CONSTANT;
		this->name = last_part;
	}	

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
