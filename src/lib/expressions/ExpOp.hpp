
#pragma once

#include "es.hpp"

class ExpOp : public Exp
{
public:
    OpType op;
    Exp *eleft;
    Exp *eright;

public:
    ExpOp(OpType op, Exp *left, Exp *right);
    ~ExpOp() override;
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpOp *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
