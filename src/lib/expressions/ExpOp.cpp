
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

    if (this->op == OpType::PAR)
        return l;

    double r = this->eright->eval(mother_sys, x);

    switch (this->op)
    {
    case OpType::ADD:
        return l + r;
    case OpType::SUB:
    case OpType::EQU:
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
    if (this->op == OpType::PAR)
        return new ExpOp(this->op, this->eleft->deep_copy(), NULL);
	return new ExpOp(this->op, this->eleft->deep_copy(), this->eright->deep_copy());
}

std::string ExpOp::to_latex() const
{
    std::string l = this->eleft->to_latex();

    if (this->op == OpType::PAR)
        return std::string("\\left(") + l + "\\right)";

    std::string r = this->eright->to_latex();

    switch (this->op)
    {
    case OpType::ADD:
        return l + " + " + r;
    case OpType::SUB:
        return l + " - " + r;
    case OpType::EQU:
        return l + " = " + r;
    case OpType::MUL:
        return l + " \\cdot " + r;
    case OpType::DIV:
	    return std::string("\\frac{") + l + "}{" + r + "}";
    case OpType::POW:
        return l + "^{" + r + "}";
    default:
        std::cerr << "Error: unknown operator: ExpOp::to_latex" << std::endl, exit(1);
    }
}

void ExpOp::print() const
{
    debug_units && std::cout << "(";

    if (this->op == OpType::PAR)
        std::cout << "(";

    this->eleft->print();

	switch (this->op)
    {
    case OpType::ADD:
        std::cout << " + ";
        break;
    case OpType::SUB:
        std::cout << " - ";
        break;
    case OpType::EQU:
        std::cout << " = ";
        break;
    case OpType::MUL:
        std::cout << " * ";
        break;
    case OpType::DIV:
        std::cout << " / ";
        break;
    case OpType::PAR:
        break;
    case OpType::POW:
        std::cout << " ^ ";
        break;
    default:
        std::cerr << "Error: unknown operator: ExpOp::print" << std::endl, exit(1);
    }

    if (this->op == OpType::PAR)
        std::cout << ")";
    else
        this->eright->print();
        
    debug_units && std::cout << ")[" << this->unit.to_string() << "]";
}

bool ExpOp::is_linear() const
{
    switch (this->op)
    {
    case OpType::ADD:
    case OpType::SUB:
    case OpType::EQU:
    case OpType::MUL:
    case OpType::DIV:
        return this->eleft->is_linear() && this->eright->is_linear();
    case OpType::PAR:
        return this->eleft->is_linear();
    case OpType::POW:
        return false;
    default:
        std::cerr << "Error: unknown operator: ExpOp::is_linear" << std::endl, exit(1);
    }
}

bool ExpOp::infer_units(std::vector<ExpVar *> &vars, SIUnit unit)
{
    if (unit.is_known && this->unit.is_known && this->unit.units != unit.units)
        std::cerr << "Error: unit mismatch: ExpOp::units_descent: beginning" << std::endl, exit(1);

    bool is_stable = true, l, r;

    if (!this->unit.is_known && unit.is_known)
    {
        this->unit = unit;
        is_stable = false;
    }

    switch (this->op)
    {
    case OpType::ADD:
    case OpType::SUB:
    case OpType::EQU:
        if (!this->unit.is_known && (this->eleft->unit.is_known || this->eright->unit.is_known))
        {
            this->unit = this->eleft->unit.is_known ? this->eleft->unit : this->eright->unit;
            is_stable = false;
        }
        l = this->eleft->infer_units(vars, this->unit);
        r = this->eright->infer_units(vars, this->unit);
        return is_stable && l && r;
    case OpType::MUL:
        if (!this->unit.is_known && this->eleft->unit.is_known && this->eright->unit.is_known)
        {
            this->unit = this->eleft->unit.multiply(this->eright->unit);
            is_stable = false;
        }
        l = this->eleft->infer_units(vars, this->eleft->unit.is_known ? this->eleft->unit : this->unit.divide(this->eright->unit));
        r = this->eright->infer_units(vars, this->eright->unit.is_known ? this->eright->unit : this->unit.divide(this->eleft->unit));
        return is_stable && l && r;
    case OpType::DIV:
        if (!this->unit.is_known && this->eleft->unit.is_known && this->eright->unit.is_known)
        {
            this->unit = this->eleft->unit.divide(this->eright->unit);
            is_stable = false;
        }
        l = this->eleft->infer_units(vars, this->eleft->unit.is_known ? this->eleft->unit : this->unit.multiply(this->eright->unit));
        r = this->eright->infer_units(vars, this->eright->unit.is_known ? this->eright->unit : this->eleft->unit.divide(this->unit));
        return is_stable && l && r;
    case OpType::PAR:
        if (!this->unit.is_known && this->eleft->unit.is_known)
        {
            this->unit = this->eleft->unit;
            is_stable = true;
        }
        l = this->eleft->infer_units(vars, this->unit);
        return is_stable && l;
    case OpType::POW:
        std::cerr << "Error: not implemented: OpType::POW: unit inferation" << std::endl, exit(1);
    default:
        std::cerr << "Error: unknown operator: ExpOp::is_linear" << std::endl, exit(1);
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

    if (this->op == OpType::PAR)
        return NULL;

    r = this->eright->singularize_vars();
    if (r != NULL)
    {
        delete this->eright;
        this->eright = r;
    }
    return NULL;
}

Exp *ExpOp::get_left()
{
    return this->eleft;
}
