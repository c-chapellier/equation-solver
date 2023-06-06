
#include "ExpPar.hpp"

ExpPar::ExpPar(Exp *left) : Exp(), eleft(left)
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

auto ExpPar::eval(System *mother_sys, const gsl_vector *x) const -> double
{
    return this->eleft->eval(mother_sys, x);
}

auto ExpPar::deep_copy() const -> ExpPar *
{
	return new ExpPar(this->eleft->deep_copy());
}

auto ExpPar::to_latex() const -> std::string
{
	return std::string("\\left(") + this->eleft->to_latex() + "\\right)";
}

void ExpPar::print() const
{
	std::cout << "(";
	this->eleft->print();
	std::cout << ")";
}
