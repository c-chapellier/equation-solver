
#include "ExpPar.hpp"

ExpPar::ExpPar(Exp *left)
    : Exp(0, "", left, NULL, NULL)
{}

double ExpPar::eval(System *mother_sys, const gsl_vector *x)
{
    return this->eleft->eval(mother_sys, x);
}

void ExpPar::replace_args(System *mother_sys, const gsl_vector *x)
{
    this->eleft->replace_args(mother_sys, x);
}

ExpPar *ExpPar::deep_copy()
{
	return new ExpPar(this->eleft->deep_copy());
}

void ExpPar::to_latex(FILE *f)
{
	fprintf(f, "\\left(");
	this->eleft->to_latex(f);
	fprintf(f, "\\right)");
}

void ExpPar::print()
{
	printf("(");
	this->eleft->print();
	printf(")");
}
