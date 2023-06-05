
#include "ExpFuncCall.hpp"

ExpFuncCall::ExpFuncCall()
{
	this->var = std::string();
	
	this->args = std::vector<Exp *>();
	
}

void ExpFuncCall::load_vars_into_sys(System *sys) const
{
	for (auto arg : this->args) {
		arg->load_vars_into_sys(sys);
}
}

void ExpFuncCall::add_arg(Exp *arg)
{
	this->args.push_back(arg);
}
