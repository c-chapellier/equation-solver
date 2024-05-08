
#include "ExpVar.hpp"

ExpVar::ExpVar(std::string var, std::string guess)
	: Exp(), var(var), guess(std::stod(guess.substr(1, guess.size() - 2)))
{

}

ExpVar *ExpVar::deep_copy() const
{
	return new ExpVar(this->var, "{" + std::to_string(this->guess) + "}");
}

std::string ExpVar::to_latex() const
{
	return Latex::var_to_latex(this->var.c_str());
}

void ExpVar::print() const
{
	std::cout << this->var;
}
