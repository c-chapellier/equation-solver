
#include "ExpMul.hpp"

ExpMul::ExpMul(Exp *left, Exp *right) : Exp()
{
    this->eleft = left;
    this->eright = right;
}

void ExpMul::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

double ExpMul::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->eleft->eval(mother_sys, x) * this->eright->eval(mother_sys, x);
}

ExpMul *ExpMul::deep_copy() const
{
	return new ExpMul(this->eleft->deep_copy(), this->eright->deep_copy());
}

std::string ExpMul::to_latex() const
{
    return this->eleft->to_latex() + " \\cdot " + this->eright->to_latex();
}

void ExpMul::print() const
{
    this->eleft->print();
    std::cout << " * ";
    this->eright->print();
}
