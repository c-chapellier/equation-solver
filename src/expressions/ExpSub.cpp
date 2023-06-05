
#include "ExpSub.hpp"

ExpSub::ExpSub(Exp *left, Exp *right) : Exp(), eleft(left), eright(right)
{
    
    
}

ExpSub::~ExpSub()
{
    delete this->eleft;
    delete this->eright;
}

void ExpSub::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

auto ExpSub::eval(System *mother_sys, const gsl_vector *x) const -> double
{
    return this->eleft->eval(mother_sys, x) - this->eright->eval(mother_sys, x);
}

auto ExpSub::deep_copy() const -> ExpSub *
{
	return new ExpSub(this->eleft->deep_copy(), this->eright->deep_copy());
}

auto ExpSub::to_latex() const -> std::string
{
	return this->eleft->to_latex() + " - " + this->eright->to_latex();
}

void ExpSub::print() const
{
    this->eleft->print();
    std::cout << " - ";
    this->eright->print();
}
