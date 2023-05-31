
#include "ExpDiv.hpp"

ExpDiv::ExpDiv(Exp *left, Exp *right)
    : Exp(0, "", left, right, NULL)
{}

double ExpDiv::eval(System *mother_sys, const gsl_vector *x)
{
    return this->eleft->eval(mother_sys, x) / this->eright->eval(mother_sys, x);
}

void ExpDiv::replace_args(System *mother_sys, const gsl_vector *x)
{
    this->eleft->replace_args(mother_sys, x);
    this->eright->replace_args(mother_sys, x);
}

ExpDiv *ExpDiv::deep_copy()
{
	return new ExpDiv(this->eleft->deep_copy(), this->eright->deep_copy());
}

void ExpDiv::to_latex(FILE *f)
{
	fprintf(f, "\\frac{");
	this->eleft->to_latex(f);
	fprintf(f, "}{");
	this->eright->to_latex(f);
	fprintf(f, "}");
}

void ExpDiv::print()
{
    this->eleft->print();
    printf(" / ");
    this->eright->print();
}
