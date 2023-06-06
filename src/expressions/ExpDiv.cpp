
#include "ExpDiv.hpp"

ExpDiv::ExpDiv(Exp *left, Exp *right) : Exp(), eleft(left), eright(right)
{
    
    
}

ExpDiv::~ExpDiv()
{
    delete this->eleft;
    delete this->eright;
}

void ExpDiv::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

auto ExpDiv::eval(System *mother_sys, const gsl_vector *x) const -> double
{
    return this->eleft->eval(mother_sys, x) / this->eright->eval(mother_sys, x);
}

auto ExpDiv::deep_copy() const -> ExpDiv *
{
	return new ExpDiv(this->eleft->deep_copy(), this->eright->deep_copy());
}

auto ExpDiv::to_latex() const -> std::string
{
	return std::string("\\frac{") + this->eleft->to_latex() + "}{" + this->eright->to_latex() + "}";
}

void ExpDiv::print() const
{
    this->eleft->print();
	std::cout << " / ";
    this->eright->print();
}
