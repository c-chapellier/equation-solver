
#include "ExpMul.hpp"

ExpMul::ExpMul(Exp *left, Exp *right) : Exp(), eleft(left), eright(right)
{
    
    
}

ExpMul::~ExpMul()
{
    delete this->eleft;
    delete this->eright;
}

void ExpMul::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

auto ExpMul::eval(System *mother_sys, const gsl_vector *x) const -> double
{
    return this->eleft->eval(mother_sys, x) * this->eright->eval(mother_sys, x);
}

auto ExpMul::deep_copy() const -> ExpMul *
{
	return new ExpMul(this->eleft->deep_copy(), this->eright->deep_copy());
}

auto ExpMul::to_latex() const -> std::string
{
    return this->eleft->to_latex() + " \\cdot " + this->eright->to_latex();
}

void ExpMul::print() const
{
    this->eleft->print();
    std::cout << " * ";
    this->eright->print();
}
