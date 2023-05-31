
#include "ExpFuncCall.hpp"

ExpFuncCall::ExpFuncCall(std::string name, System *sys) : Exp()
{
	this->var = name;
	this->sys = sys;
}

void ExpFuncCall::load_vars_into_sys(System *sys) const
{
	for (int i = 0; i < this->args.size(); ++i)
		this->args[i]->load_vars_into_sys(sys);
}

void ExpFuncCall::to_latex(std::ofstream &f) const
{
	Latex::var_to_latex(f, this->var);

	f << "\\left(\n";
	for (int i = 0; i < this->args.size(); ++i)
	{
		this->args[i]->to_latex(f);
		if (i != this->args.size() - 1)
			f << ", ";
	}
	f << "\\right)";
}

void ExpFuncCall::add_arg(Exp *arg)
{
	this->args.push_back(arg);
}
