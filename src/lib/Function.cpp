
#include "Function.hpp"

Function::Function(std::string name, std::vector<std::string> args_names, System *sys, Exp *ret)
    : name(name), args_names(args_names), sys(sys), ret(ret), call_count(0)
{

}

Function::~Function()
{
	ExpOp *op = dynamic_cast<ExpOp *>(this->ret);
	if (op != nullptr)
		this->sys->add_equ(op);
	else
		delete this->ret;
	
	delete this->sys;
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
	std::string out = Latex::var_to_latex(this->name) + "(";

	for (int i = 0; i < this->args_names.size(); ++i)
	{
		out += this->args_names[i];
		if (i != this->args_names.size() - 1)
			out += ", ";
	}

	out += ")";
	if (this->sys->equs.size() > 0)
		out += ":";

	for (int i = 0; i < this->sys->equs.size(); ++i)
		out += this->sys->equs[i]->to_latex() + " ; ";

	return out + " \\rArr " + this->ret->to_latex();
}
