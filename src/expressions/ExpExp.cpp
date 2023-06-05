
#include "ExpExp.hpp"

ExpExp::ExpExp(Exp *left, Exp *right) : Exp(), eleft(left), eright(right)
{
    
    
}

ExpExp::~ExpExp()
{
    delete this->eleft;
    delete this->eright;
}

void ExpExp::load_vars_into_sys(System *sys) const
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

auto ExpExp::eval(System *mother_sys, const gsl_vector *x) const -> double
{
    return pow(this->eleft->eval(mother_sys, x), this->eright->eval(mother_sys, x));
}

auto ExpExp::deep_copy() const -> ExpExp *
{
	return new ExpExp(this->eleft->deep_copy(), this->eright->deep_copy());
}

auto ExpExp::to_latex() const -> std::string
{
	return this->eleft->to_latex() + "^{" + this->eright->to_latex() + "}";
}

void ExpExp::print() const
{
    this->eleft->print();
	std::cout << "^";
    this->eright->print();
}
