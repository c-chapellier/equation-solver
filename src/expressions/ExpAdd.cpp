
#include "ExpAdd.hpp"

ExpAdd::ExpAdd(Exp *left, Exp *right) : Exp()
{
    this->eleft = left;
    this->eright = right;
}

ExpAdd::~ExpAdd()
{
    delete this->eleft;
    delete this->eright;
}

void ExpAdd::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

double ExpAdd::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->eleft->eval(mother_sys, x) + this->eright->eval(mother_sys, x);
}

ExpAdd *ExpAdd::deep_copy() const
{
	return new ExpAdd(this->eleft->deep_copy(), this->eright->deep_copy());
}

std::string ExpAdd::to_latex() const
{
	return this->eleft->to_latex() + " + " + this->eright->to_latex();
}

void ExpAdd::print() const
{
    this->eleft->print();
    std::cout << " + ";
    this->eright->print();
}
