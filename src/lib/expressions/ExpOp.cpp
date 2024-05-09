
#include "ExpOp.hpp"

ExpOp::ExpOp(OpType op, Exp *left, Exp *right)
    : Exp(), op(op), eleft(left), eright(right)
{

}

ExpOp::~ExpOp()
{
    ExpVar *var = static_cast<ExpVar *>(this->eleft);
    if (var == NULL)
        delete this->eleft;
    var = static_cast<ExpVar *>(this->eright);
    if (var == NULL)
        delete this->eright;
}

double ExpOp::eval(System *mother_sys, const gsl_vector *x) const
{
    double l = this->eleft->eval(mother_sys, x);
    double r = this->eright->eval(mother_sys, x);

    switch (this->op)
    {
    case OpType::ADD:
        return l + r;
    case OpType::SUB:
        return l - r;
    case OpType::MUL:
        return l * r;
    case OpType::DIV:
        return l / r;
    case OpType::POW:
        return pow(l, r);
    default:
        std::cerr << "Error: unknown operator: ExpOp::eval" << std::endl, exit(1);
    }
}

ExpOp *ExpOp::deep_copy() const
{
	return new ExpOp(this->op, this->eleft->deep_copy(), this->eright->deep_copy());
}

std::string ExpOp::to_latex() const
{
    switch (this->op)
    {
    case OpType::ADD:
        return this->eleft->to_latex() + " + " + this->eright->to_latex();
    case OpType::SUB:
        return this->eleft->to_latex() + " - " + this->eright->to_latex();
    case OpType::MUL:
        return this->eleft->to_latex() + " \\cdot " + this->eright->to_latex();
    case OpType::DIV:
	    return std::string("\\frac{") + this->eleft->to_latex() + "}{" + this->eright->to_latex() + "}";
    case OpType::POW:
        return this->eleft->to_latex() + "^{" + this->eright->to_latex() + "}";
    default:
        std::cerr << "Error: unknown operator: ExpOp::to_latex" << std::endl, exit(1);
    }
}

void ExpOp::print() const
{
    debug_units && std::cout << "(";
    this->eleft->print();
	switch (this->op)
    {
    case OpType::ADD:
        std::cout << " + ";
        break;
    case OpType::SUB:
        std::cout << " - ";
        break;
    case OpType::MUL:
        std::cout << " * ";
        break;
    case OpType::DIV:
        std::cout << " / ";
        break;
    case OpType::POW:
        std::cout << " ^ ";
        break;
    default:
        std::cerr << "Error: unknown operator: ExpOp::print" << std::endl, exit(1);
    }
    this->eright->print();
    debug_units && std::cout << ")[" << this->unit.to_string() << "]";
}

bool ExpOp::is_linear() const
{
    switch (this->op)
    {
    case OpType::ADD:
    case OpType::SUB:
    case OpType::MUL:
    case OpType::DIV:
        return this->eleft->is_linear() && this->eright->is_linear();
    case OpType::POW:
        return false;
    default:
        std::cerr << "Error: unknown operator: ExpOp::is_linear" << std::endl, exit(1);
    }
}

std::vector<ExpVar *> ExpOp::get_vars()
{
    switch (this->op)
    {
    case OpType::ADD:
    case OpType::SUB:
        if (this->eleft->unit.unit_known && this->eright->unit.unit_known)
        {
            if (this->eleft->unit.units != this->eright->unit.units)
            {
                std::cerr << "Error: unit mismatch: ExpOp::get_vars: '" << this->eleft->unit.to_string();
                std::cerr << "' != '" << this->eright->unit.to_string() << "'" << std::endl;
                exit(1);
            }
            this->unit = this->eleft->unit;
        }
        else if (this->eleft->unit.unit_known || this->eright->unit.unit_known)
            this->unit = this->eleft->unit.unit_known ? this->eleft->unit : this->eright->unit;
        else { /* stay unknown */ }
        break;
    case OpType::MUL:
        if (this->eleft->unit.unit_known && this->eright->unit.unit_known)
        {
            this->unit.unit_known = true;
            for (auto &unit : this->unit.units)
                this->unit.units[unit.first] = this->eleft->unit.units[unit.first] + this->eright->unit.units[unit.first];
        }
        else { /* stay unknown */ }
        break;
    case OpType::DIV:
        if (this->eleft->unit.unit_known && this->eright->unit.unit_known)
        {
            this->unit.unit_known = true;
            for (auto &unit : this->unit.units)
                this->unit.units[unit.first] = this->eleft->unit.units[unit.first] - this->eright->unit.units[unit.first];
        }
        else { /* stay unknown */ }
        break;
    case OpType::POW:
        std::cerr << "Error: not implemented: OpType::POW: unit inferation" << std::endl, exit(1);
    default:
        std::cerr << "Error: unknown operator: ExpOp::is_linear" << std::endl, exit(1);
    }

    std::vector<ExpVar *> vars = this->eleft->get_vars();
    std::vector<ExpVar *> right_vars = this->eright->get_vars();
    vars.insert(vars.end(), right_vars.begin(), right_vars.end());
    return vars;
}

void ExpOp::units_descent(SIUnit unit)
{
    if (unit.unit_known && this->unit.unit_known && this->unit.units != unit.units)
        std::cerr << "Error: unit mismatch: ExpOp::units_descent: beginning: '" << this->unit.to_string() << "' != '" << unit.to_string() << "'" << std::endl, exit(1);

    if (!this->unit.unit_known && unit.unit_known)
        this->unit = unit;

    switch (this->op)
    {
    case OpType::ADD:
    case OpType::SUB:
        this->eleft->units_descent(this->unit);
        this->eright->units_descent(this->unit);
        break;
    case OpType::MUL:
        if (this->eleft->unit.unit_known && this->eright->unit.unit_known)
        {
            if (this->unit.unit_known)
            {
                if (this->unit.units != this->eleft->unit.multiply(this->eright->unit).units)
                {
                    std::cerr << "Error: unit mismatch: ExpOp::units_descent: mult: '" << this->unit.to_string();
                    std::cerr << "' != '" << this->eleft->unit.multiply(this->eright->unit).to_string() << "'" << std::endl;
                    exit(1);
                }
            }
            else
            {
                this->unit = this->eleft->unit.multiply(this->eright->unit);
            }
        }
        else if (this->eleft->unit.unit_known || this->eright->unit.unit_known)
        {
            if (this->eleft->unit.unit_known)
                this->eright->units_descent(this->unit.divide(this->eleft->unit));
            else
                this->eleft->units_descent(this->unit.divide(this->eright->unit));
        }
        else { /* stay unknown */ }
        break;
    case OpType::DIV:
        if (this->eleft->unit.unit_known && this->eright->unit.unit_known)
        {
            if (this->unit.unit_known)
            {
                if (this->unit.units != this->eleft->unit.divide(this->eright->unit).units)
                {
                    std::cerr << "Error: unit mismatch: ExpOp::units_descent: div: '" << this->unit.to_string();
                    std::cerr << "' != '" << this->eleft->unit.divide(this->eright->unit).to_string() << "'" << std::endl;
                    exit(1);
                }
            }
            else
            {
                this->unit = this->eleft->unit.divide(this->eright->unit);
            }
        }
        else if (this->eleft->unit.unit_known || this->eright->unit.unit_known)
        {
            if (this->eleft->unit.unit_known)
                this->eright->units_descent(this->eleft->unit.divide(this->unit));
            else
                this->eleft->units_descent(this->eright->unit.multiply(this->unit));
        }
        else { /* stay unknown */ }
        break;
    case OpType::POW:
        std::cerr << "Error: not implemented: OpType::POW: units_descent" << std::endl, exit(1);
    default:
        std::cerr << "Error: unknown operator: ExpOp::units_descent" << std::endl, exit(1);
    }
}

Exp *ExpOp::singularize_vars()
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
