
#include "ExpEqu.hpp"

ExpEqu::ExpEqu(Exp *left, Exp *right)
    : Exp(), eleft(left), eright(right)
{

}

ExpEqu::~ExpEqu()
{
    ExpVar *var = static_cast<ExpVar *>(this->eleft);
    if (var == NULL)
        delete this->eleft;
    var = static_cast<ExpVar *>(this->eright);
    if (var == NULL)
        delete this->eright;
}

double ExpEqu::eval(System *mother_sys, const gsl_vector *x) const
{
    return this->eleft->eval(mother_sys, x) - this->eright->eval(mother_sys, x);
}

ExpEqu *ExpEqu::deep_copy() const
{
	return new ExpEqu(this->eleft->deep_copy(), this->eright->deep_copy());
}

std::string ExpEqu::to_latex() const
{
    return this->eleft->to_latex() + " = " + this->eright->to_latex();
}

void ExpEqu::print() const
{
    debug_units && std::cout << "(";
    this->eleft->print();
    std::cout << " = ";
    this->eright->print();
    debug_units && std::cout << ")[" << this->unit.to_string() << "]";
}

Exp *ExpEqu::get_left()
{
    return this->eleft;
}

bool ExpEqu::is_linear() const
{
    return this->eleft->is_linear() && this->eright->is_linear();
}

std::vector<ExpVar *> ExpEqu::get_vars()
{
    if (this->eleft->unit.unit_known && this->eright->unit.unit_known)
    {
        if (this->eleft->unit.units != this->eright->unit.units)
        {
            std::cerr << "Error: unit mismatch: ExpEqu::get_vars: '" << this->eleft->unit.to_string();
            std::cerr << "' != '" << this->eright->unit.to_string() << "'" << std::endl;
            exit(1);
        }
        this->unit = this->eleft->unit;
    }
    else if (this->eleft->unit.unit_known || this->eright->unit.unit_known)
        this->unit = this->eleft->unit.unit_known ? this->eleft->unit : this->eright->unit;
    else { /* stay unknown */ }
        
    std::vector<ExpVar *> vars = this->eleft->get_vars();
    std::vector<ExpVar *> right_vars = this->eright->get_vars();
    vars.insert(vars.end(), right_vars.begin(), right_vars.end());
    return vars;
}

void ExpEqu::units_descent(SIUnit unit)
{
    this->eleft->units_descent(unit);
    this->eright->units_descent(unit);
}

Exp *ExpEqu::singularize_vars()
{
    Exp *r = this->eleft->singularize_vars();
    if (r != NULL)
    {
        delete this->eleft;
        this->eleft = r;
    }
    r = this->eright->singularize_vars();
    if (r != NULL)
    {
        delete this->eright;
        this->eright = r;
    }
    return NULL;
}
