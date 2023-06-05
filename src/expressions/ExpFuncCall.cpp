
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
<<<<<<< HEAD
	for (auto *arg : this->args) {
		arg->load_vars_into_sys(sys);
}
=======
	for (int i = 0; i < this->args.size(); ++i)
		this->args[i]->load_vars_into_sys(sys);
>>>>>>> parent of a08e814 (run static analyzer)
}

void ExpFuncCall::add_arg(Exp *arg)
{
	this->args.push_back(arg);
}
