
#include "ExpMul.hpp"

ExpMul::ExpMul(Exp *left, Exp *right) : Exp()
{
    this->eleft = left;
    this->eright = right;
}

double ExpMul::eval(System *mother_sys, const gsl_vector *x)
{
    return this->eleft->eval(mother_sys, x) * this->eright->eval(mother_sys, x);
}

ExpMul *ExpMul::deep_copy()
{
	return new ExpMul(this->eleft->deep_copy(), this->eright->deep_copy());
}

void ExpMul::to_latex(FILE *f)
{
    this->eleft->to_latex(f);
	fprintf(f, " \\cdot ");
	this->eright->to_latex(f);
}

void ExpMul::print()
{
    this->eleft->print();
    printf(" * ");
    this->eright->print();
}
