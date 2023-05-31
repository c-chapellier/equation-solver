
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpDiv : public Exp
{
public:
    Exp *eleft;
    Exp *eright;

public:
    ExpDiv(Exp *left, Exp *right);
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpDiv *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
