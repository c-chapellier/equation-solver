
#include "ExpPar.hpp"

ExpPar::ExpPar(Exp *left)
	: Exp(), eleft(left)
{

}

ExpPar::~ExpPar()
{
    ExpVar *var = static_cast<ExpVar *>(this->eleft);
    if (var == NULL)
        delete this->eleft;
}

double ExpPar::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->eleft->eval(mother_sys, x);
}

ExpPar *ExpPar::deep_copy() const
{
	return new ExpPar(this->eleft->deep_copy());
}

std::string ExpPar::to_latex() const
{
	return std::string("\\left(") + this->eleft->to_latex() + "\\right)";
}

void ExpPar::print() const
{
	std::cout << "(";
	this->eleft->print();
	std::cout << ")";
    debug_units && std::cout << "[" << this->unit.to_string() << "]";
}

bool ExpPar::is_linear() const
{
    return this->eleft->is_linear();
}

std::vector<ExpVar *> ExpPar::units_ascent()
{
	this->unit = this->eleft->unit;
	return this->eleft->units_ascent();
}

void ExpPar::units_descent(SIUnit unit)
{
	if (this->unit.is_known && unit.is_known && this->unit.units != unit.units)
		std::cerr << "Error: units mismatch in ExpPar" << std::endl;

	if (!this->unit.is_known && unit.is_known)
		this->unit = unit;

	if (this->unit.is_known && !unit.is_known)
		unit = this->unit;

	this->eleft->units_descent(unit);
}

Exp *ExpPar::singularize_vars()
{
    Exp *r = this->eleft->singularize_vars();
	if (r != NULL)
	{
		delete this->eleft;
		this->eleft = NULL;
	}
    return NULL;
}
