
#include "ExpSub.hpp"

ExpSub::ExpSub(Exp *left, Exp *right) : Exp()
{
    this->eleft = left;
    this->eright = right;
}

void ExpSub::load_vars_into_sys(System *sys)
{
	this->eleft->load_vars_into_sys(sys);
	this->eright->load_vars_into_sys(sys);
}

double ExpSub::eval(System *mother_sys, const gsl_vector *x)
{
    return this->eleft->eval(mother_sys, x) - this->eright->eval(mother_sys, x);
}

ExpSub *ExpSub::deep_copy()
{
	return new ExpSub(this->eleft->deep_copy(), this->eright->deep_copy());
}

void ExpSub::to_latex(FILE *f)
{
	this->eleft->to_latex(f);
	fprintf(f, " - ");
	this->eright->to_latex(f);
}

void ExpSub::print()
{
    this->eleft->print();
    printf(" - ");
    this->eright->print();
}
