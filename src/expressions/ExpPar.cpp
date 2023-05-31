
#include "ExpPar.hpp"

ExpPar::ExpPar(Exp *left) : Exp()
{
	this->eleft = left;
}

void ExpPar::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
}

double ExpPar::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->eleft->eval(mother_sys, x);
}

ExpPar *ExpPar::deep_copy() const
{
	return new ExpPar(this->eleft->deep_copy());
}

void ExpPar::to_latex(std::ofstream &f) const
{
	f << "\\left(";
	this->eleft->to_latex(f);
	f << "\\right)";
}

void ExpPar::print() const
{
	std::cout << "(";
	this->eleft->print();
	std::cout << ")";
}
