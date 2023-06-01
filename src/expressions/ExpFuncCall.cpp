
#include "ExpFuncCall.hpp"

ExpFuncCall::ExpFuncCall()
{
	this->var = std::string();
	this->sys = nullptr;
	this->args = std::vector<Exp *>();
	this->f = nullptr;
}

void ExpFuncCall::load_vars_into_sys(System *sys) const
{
	for (int i = 0; i < this->args.size(); ++i)
		this->args[i]->load_vars_into_sys(sys);
}

void ExpFuncCall::add_arg(Exp *arg)
{
	this->args.push_back(arg);
}
