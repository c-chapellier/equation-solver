
#include "ExpPar.hpp"

ExpPar::ExpPar(Exp *left) : Exp()
{
	this->eleft = left;
}

void ExpPar::load_vars_into_sys(System *sys)
{
	this->eleft->load_vars_into_sys(sys);
}

double ExpPar::eval(System *mother_sys, const gsl_vector *x)
{
    return this->eleft->eval(mother_sys, x);
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
