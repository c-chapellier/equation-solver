
#include "ExpOp.hpp"

ExpOp::ExpOp(OpType op, Exp *left, Exp *right)
    : Exp(), op(op), eleft(left), eright(right)
{

}

ExpOp::~ExpOp()
{
    ExpVar *v = dynamic_cast<ExpVar *>(this->eleft);
    if (v == NULL)
        delete this->eleft;
	
    if (this->op != OpType::PAR)
    {
        v = dynamic_cast<ExpVar *>(this->eright);
        if (v == NULL)
            delete this->eright;
    }
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

bool ExpOp::infer_units(std::vector<ExpVar *> &vars, siu_t unit, bool is_value_known, double value)
{
    if (this->unit.is_known && unit.is_known)
        assert(siu_compare(this->unit, unit));
    
    if (this->is_value_known && is_value_known)
        assert(abs(this->value - value) < 1e-6);
    
    bool is_stable = true, l, r;

    if (!this->unit.is_known && unit.is_known)
    {
        if (!this->is_value_known && is_value_known)
        {
            this->value = value;
            this->is_value_known = true;
        }
        this->unit = unit;
        is_stable = false;
    }

    switch (this->op)
    {
    case OpType::ADD:
        if (!this->is_value_known && this->eleft->is_value_known && this->eright->is_value_known)
        {
            this->value = this->eleft->value + this->eright->value;
            this->is_value_known = true;
            is_stable = false;
        }
        if (!this->unit.is_known && (this->eleft->unit.is_known || this->eright->unit.is_known))
        {
            this->unit = this->eleft->unit.is_known ? this->eleft->unit : this->eright->unit;
            is_stable = false;
        }
        l = this->eleft->infer_units(vars, this->unit, this->is_value_known && this->eright->is_value_known, this->value - this->eright->value);
        r = this->eright->infer_units(vars, this->unit, this->is_value_known && this->eleft->is_value_known, this->value - this->eleft->value);
        return is_stable && l && r;
    case OpType::SUB:
        if (!this->is_value_known && this->eleft->is_value_known && this->eright->is_value_known)
        {
            this->value = this->eleft->value - this->eright->value;
            this->is_value_known = true;
            is_stable = false;
        }
        if (!this->unit.is_known && (this->eleft->unit.is_known || this->eright->unit.is_known))
        {
            this->unit = this->eleft->unit.is_known ? this->eleft->unit : this->eright->unit;
            is_stable = false;
        }
        l = this->eleft->infer_units(vars, this->unit, this->is_value_known && this->eright->is_value_known, this->value + this->eright->value);
        r = this->eright->infer_units(vars, this->unit, this->is_value_known && this->eleft->is_value_known, this->eleft->value - this->value);
        return is_stable && l && r;
    case OpType::EQU:
        if (!this->is_value_known && (this->eleft->is_value_known || this->eright->is_value_known))
        {
            this->value = this->eleft->is_value_known ? this->eleft->value : this->eright->value;
            this->is_value_known = true;
            is_stable = false;
        }
        if (!this->unit.is_known && (this->eleft->unit.is_known || this->eright->unit.is_known))
        {
            this->unit = this->eleft->unit.is_known ? this->eleft->unit : this->eright->unit;
            is_stable = false;
        }
        l = this->eleft->infer_units(vars, this->unit, this->is_value_known && this->eright->is_value_known, this->value);
        r = this->eright->infer_units(vars, this->unit, this->is_value_known && this->eleft->is_value_known, this->value);
        return is_stable && l && r;
    case OpType::MUL:
        if (!this->is_value_known && this->eleft->is_value_known && this->eright->is_value_known)
        {
            this->value = this->eleft->value * this->eright->value;
            this->is_value_known = true;
            is_stable = false;
        }
        if (!this->unit.is_known && this->eleft->unit.is_known && this->eright->unit.is_known)
        {
            this->unit = siu_multiply(this->eleft->unit, this->eright->unit);
            is_stable = false;
        }
        l = this->eleft->infer_units(
            vars,
            this->eleft->unit.is_known ? this->eleft->unit : siu_divide(this->unit, this->eright->unit),
            this->is_value_known && this->eright->is_value_known,
            this->value / this->eright->value
        );
        r = this->eright->infer_units(
            vars,
            this->eright->unit.is_known ? this->eright->unit : siu_divide(this->unit, this->eleft->unit),
            this->is_value_known && this->eleft->is_value_known,
            this->value / this->eleft->value
        );
        return is_stable && l && r;
    case OpType::DIV:
        if (!this->is_value_known && this->eleft->is_value_known && this->eright->is_value_known)
        {
            this->value = this->eleft->value / this->eright->value;
            this->is_value_known = true;
            is_stable = false;
        }
        if (!this->unit.is_known && this->eleft->unit.is_known && this->eright->unit.is_known)
        {
            this->unit = siu_divide(this->eleft->unit, this->eright->unit);
            is_stable = false;
        }
        l = this->eleft->infer_units(
            vars,
            this->eleft->unit.is_known ? this->eleft->unit : siu_multiply(this->unit, this->eright->unit),
            this->is_value_known && this->eright->is_value_known,
            this->value * this->eright->value
        );
        r = this->eright->infer_units(
            vars,
            this->eright->unit.is_known ? this->eright->unit : siu_divide(this->eleft->unit, this->unit),
            this->is_value_known && this->eleft->is_value_known,
            this->eleft->value / this->value
        );
        return is_stable && l && r;
    case OpType::PAR:
        if (!this->is_value_known && this->eleft->is_value_known)
        {
            this->value = this->eleft->value;
            this->is_value_known = true;
            is_stable = false;
        }
        if (!this->unit.is_known && this->eleft->unit.is_known)
        {
            this->unit = this->eleft->unit;
            is_stable = false;
        }
        l = this->eleft->infer_units(vars, this->unit, this->is_value_known, this->value);
        return is_stable && l;
    case OpType::POW:
        std::cerr << "Error: not implemented: OpType::POW: unit inferation" << std::endl, exit(1);
    default:
        std::cerr << "Error: unknown operator: ExpOp::is_linear" << std::endl, exit(1);
    }
}

Exp *ExpOp::singularize_vars(System *sys)
{
    Exp *r = this->eleft->singularize_vars(sys);
    if (r != NULL)
        this->eleft = r;

    if (this->op == OpType::PAR)
        return NULL;

    r = this->eright->singularize_vars(sys);
    if (r != NULL)
        this->eright = r;

    return NULL;
}

Exp *ExpOp::get_left()
{
    return this->eleft;
}

bool ExpOp::is_completly_infered() const
{
    if (!this->is_value_known)
        return false;
    if (this->op == OpType::PAR)
        return this->eleft->is_completly_infered();
    return this->eleft->is_completly_infered() && this->eright->is_completly_infered();
}

std::ostream &ExpOp::output(std::ostream &os) const
{
    os << "(";

    if (this->op == OpType::PAR)
        os << "(";

    os << *this->eleft;

	switch (this->op)
    {
    case OpType::ADD:
        os << " + ";
        break;
    case OpType::SUB:
        os << " - ";
        break;
    case OpType::EQU:
        os << " = ";
        break;
    case OpType::MUL:
        os << " * ";
        break;
    case OpType::DIV:
        os << " / ";
        break;
    case OpType::PAR:
        break;
    case OpType::POW:
        os << " ^ ";
        break;
    default:
        std::cerr << "Error: unknown operator: ExpOp::print" << std::endl, exit(1);
    }

    if (this->op == OpType::PAR)
        os << ")";
    else
        os << *this->eright;
        
    os << ")["<< this->value <<  "|" << siu_to_latex(this->unit) << "]";
    return os;
}

void ExpOp::add_equs_from_func_calls(System *sys)
{
    this->eleft->add_equs_from_func_calls(sys);
    if (this->op == OpType::PAR)
        return ;
    this->eright->add_equs_from_func_calls(sys);
}

void ExpOp::add_prefix_to_vars(std::string prefix)
{
    this->eleft->add_prefix_to_vars(prefix);
    if (this->op == OpType::PAR)
        return ;
    this->eright->add_prefix_to_vars(prefix);
}
