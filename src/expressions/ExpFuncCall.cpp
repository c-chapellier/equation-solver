
#include "ExpFuncCall.hpp"

ExpFuncCall::ExpFuncCall(std::string name, System *sys) : Exp()
{
	this->var = name;
	this->sys = sys;
}

void ExpFuncCall::to_latex(FILE *f)
{
	Latex::var_to_latex(f, this->var.c_str());
	fprintf(f, "\\left(\n");
	for (int i = 0; i < this->args.size(); ++i)
	{
		this->args[i]->to_latex(f);
		if (i != this->args.size() - 1)
			fprintf(f, ", ");
	}
	fprintf(f, "\\right)\n");
}

void ExpFuncCall::add_arg(Exp *arg)
{
	this->args.push_back(arg);
}
