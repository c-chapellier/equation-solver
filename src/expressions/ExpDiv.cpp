
#include "ExpDiv.hpp"

ExpDiv::ExpDiv(Exp *left, Exp *right) : Exp()
{
    this->eleft = left;
    this->eright = right;
}

void ExpDiv::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

double ExpDiv::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->eleft->eval(mother_sys, x) / this->eright->eval(mother_sys, x);
}

ExpDiv *ExpDiv::deep_copy() const
{
	return new ExpDiv(this->eleft->deep_copy(), this->eright->deep_copy());
}

std::string ExpDiv::to_latex() const
{
	return std::string("\\frac{") + this->eleft->to_latex() + "}{" + this->eright->to_latex() + "}";
}

void ExpDiv::print() const
{
    this->eleft->print();
	std::cout << " / ";
    this->eright->print();
}
