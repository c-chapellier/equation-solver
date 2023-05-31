
#include "ExpAdd.hpp"

ExpAdd::ExpAdd(Exp *left, Exp *right)
    : Exp(0, "", left, right, NULL)
{}

double ExpAdd::eval(System *mother_sys, const gsl_vector *x)
{
    return this->eleft->eval(mother_sys, x) + this->eright->eval(mother_sys, x);
}

void ExpAdd::replace_args(System *mother_sys, const gsl_vector *x)
{
    this->eleft->replace_args(mother_sys, x);
    this->eright->replace_args(mother_sys, x);
}

ExpAdd *ExpAdd::deep_copy()
{
	return new ExpAdd(this->eleft->deep_copy(), this->eright->deep_copy());
}

void ExpAdd::to_latex(FILE *f)
{
	this->eleft->to_latex(f);
	fprintf(f, " + ");
	this->eright->to_latex(f);
}

void ExpAdd::print()
{
    this->eleft->print();
    printf(" + ");
    this->eright->print();
}
