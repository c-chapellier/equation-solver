
#include "ExpEqu.hpp"

ExpEqu::ExpEqu(Exp *left, Exp *right) : Exp()
{
    this->eleft = left;
    this->eright = right;
}

void ExpEqu::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

double ExpEqu::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->eleft->eval(mother_sys, x) - this->eright->eval(mother_sys, x);
}

ExpEqu *ExpEqu::deep_copy() const
{
	return new ExpEqu(this->eleft->deep_copy(), this->eright->deep_copy());
}

void ExpEqu::to_latex(std::ofstream &f) const
{
	this->eleft->to_latex(f);
    f << " = ";
	this->eright->to_latex(f);
}

void ExpEqu::print() const
{
    this->eleft->print();
    std::cout << " = ";
    this->eright->print();
}
