
#include "Function.hpp"

Function::Function(std::string name, std::vector<std::string> args_names, system_t *sys, Exp *ret)
    : name(name), args_names(args_names), sys(sys), ret(ret), call_count(0)
{

}

void Function::print() const
{
	std::cout << "Function: " << this->name << std::endl;
	std::cout << "Args: ";
	for (int i = 0; i < this->args_names.size(); ++i)
		std::cout << this->args_names[i] << " ";
	std::cout << std::endl;
	std::cout << "Fn system: " << std::endl;
	std::cout << this->sys;
	std::cout << "Return: " << std::endl;
	std::cout << this->ret << std::endl;
}

std::string Function::to_latex() const
{
	std::string res = "";
	res += "" + Latex::var_to_latex(this->name) + "(";
	for (int i = 0; i < this->args_names.size(); ++i)
	{
		res += this->args_names.at(i);
		if (i != this->args_names.size() - 1)
			res += ", ";
	}
	res += ")";
	if (this->sys->n_equs > 0)
		res += ":";
	for (int i = 0; i < this->sys->n_equs; ++i)
		res += this->sys->equs[i]->to_latex() + " ; ";
	res += " \\rArr " + this->ret->to_latex() + "";
	return res;
}

Function::~Function()
{
	sys_free(this->sys);
	delete this->ret;
}
