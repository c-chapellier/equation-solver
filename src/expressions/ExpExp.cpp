
#include "ExpExp.hpp"

ExpExp::ExpExp(Exp *left, Exp *right) : Exp()
{
    this->eleft = left;
    this->eright = right;
}

double ExpExp::eval(System *mother_sys, const gsl_vector *x)
{
    return pow(this->eleft->eval(mother_sys, x), this->eright->eval(mother_sys, x));
}

ExpExp *ExpExp::deep_copy()
{
	return new ExpExp(this->eleft->deep_copy(), this->eright->deep_copy());
}

void ExpExp::to_latex(FILE *f)
{
	this->eleft->to_latex(f);
	fprintf(f, "^{");
	this->eright->to_latex(f);
	fprintf(f, "}");
}

void ExpExp::print()
{
    this->eleft->print();
    printf(" ^ ");
    this->eright->print();
}
