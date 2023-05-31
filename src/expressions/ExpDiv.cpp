
#include "ExpDiv.hpp"

ExpDiv::ExpDiv(Exp *left, Exp *right) : Exp()
{
    this->eleft = left;
    this->eright = right;
}

double ExpDiv::eval(System *mother_sys, const gsl_vector *x)
{
    return this->eleft->eval(mother_sys, x) / this->eright->eval(mother_sys, x);
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
