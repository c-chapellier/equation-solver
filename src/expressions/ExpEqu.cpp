
#include "ExpEqu.hpp"

ExpEqu::ExpEqu(Exp *left, Exp *right) : Exp(), eleft(left), eright(right)
{
    
    
}

ExpEqu::~ExpEqu()
{
    delete this->eleft;
    delete this->eright;
}

void ExpEqu::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

auto ExpEqu::eval(System *mother_sys, const gsl_vector *x) const -> double
{
    return this->eleft->eval(mother_sys, x) - this->eright->eval(mother_sys, x);
}

auto ExpEqu::deep_copy() const -> ExpEqu *
{
	return new ExpEqu(this->eleft->deep_copy(), this->eright->deep_copy());
}

auto ExpEqu::to_latex() const -> std::string
{
	return this->eleft->to_latex() + " = " + this->eright->to_latex();
}

void ExpEqu::print() const
{
    this->eleft->print();
    std::cout << " = ";
    this->eright->print();
}
