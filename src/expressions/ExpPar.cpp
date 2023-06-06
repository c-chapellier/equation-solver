
#include "ExpPar.hpp"

ExpPar::ExpPar(Exp *left)
	: Exp(), eleft(left)
{

}

ExpPar::~ExpPar()
{
	delete this->eleft;
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

std::string ExpPar::to_latex() const
{
	return std::string("\\left(") + this->eleft->to_latex() + "\\right)";
}

void ExpPar::print() const
{
	std::cout << "(";
	this->eleft->print();
	std::cout << ")";
}
