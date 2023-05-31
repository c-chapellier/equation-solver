
#include "ExpExp.hpp"

ExpExp::ExpExp(Exp *left, Exp *right) : Exp()
{
    this->eleft = left;
    this->eright = right;
}

void ExpExp::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

double ExpExp::eval(System *mother_sys, const gsl_vector *x) const
{
    return pow(this->eleft->eval(mother_sys, x), this->eright->eval(mother_sys, x));
}

ExpExp *ExpExp::deep_copy() const
{
	return new ExpExp(this->eleft->deep_copy(), this->eright->deep_copy());
}

void ExpExp::to_latex(std::ofstream &f) const
{
	this->eleft->to_latex(f);
	f << "^{";
	this->eright->to_latex(f);
	f << "}";
}

void ExpExp::print() const
{
    this->eleft->print();
	std::cout << "^";
    this->eright->print();
}
