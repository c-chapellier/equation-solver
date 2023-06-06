
#include "ExpSub.hpp"

ExpSub::ExpSub(Exp *left, Exp *right)
    : Exp(), eleft(left), eright(right)
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

double ExpSub::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->eleft->eval(mother_sys, x) - this->eright->eval(mother_sys, x);
}

ExpSub *ExpSub::deep_copy() const
{
	return new ExpSub(this->eleft->deep_copy(), this->eright->deep_copy());
}

std::string ExpSub::to_latex() const
{
	return this->eleft->to_latex() + " - " + this->eright->to_latex();
}

void ExpSub::print() const
{
    this->eleft->print();
    std::cout << " - ";
    this->eright->print();
}
