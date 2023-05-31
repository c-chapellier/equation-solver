
#include "ExpFuncCall.hpp"

void ExpFuncCall::load_vars_into_sys(System *sys) const
{
	for (int i = 0; i < this->args.size(); ++i)
		this->args[i]->load_vars_into_sys(sys);
}

void ExpFuncCall::add_arg(Exp *arg)
{
	this->args.push_back(arg);
}
