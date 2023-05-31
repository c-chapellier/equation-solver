
#include "Exp.hpp"

Exp::Exp(double dvalue, std::string var, Exp *left, Exp *right, System *sys)
{
	this->dval = dvalue;
	this->var = var;
	this->eleft = left;
	this->eright = right;
	this->sys = sys;
}

void Exp::add_arg(Exp *arg)
{
	this->args.push_back(arg);
}

