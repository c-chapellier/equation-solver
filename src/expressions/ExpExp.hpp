
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpExp : public Exp
{
public:
    Exp *eleft;
    Exp *eright;
    
public:
    ExpExp(Exp *left, Exp *right);
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpExp *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
