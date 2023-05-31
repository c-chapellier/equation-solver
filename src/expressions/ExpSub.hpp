
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpSub : public Exp
{
public:
    Exp *eleft;
    Exp *eright;
    
public:
    ExpSub(Exp *left, Exp *right);
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpSub *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
