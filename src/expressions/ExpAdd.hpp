
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpAdd : public Exp
{
public:
    Exp *eleft;
    Exp *eright;

public:
    ExpAdd(Exp *left, Exp *right);
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpAdd *deep_copy() const override;
    void to_latex(std::ofstream &f) const override;
    void print() const override;
};
