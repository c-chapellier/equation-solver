
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpEqu : public Exp
{
public:
    Exp *eleft;
    Exp *eright;
    
public:
    ExpEqu(Exp *left, Exp *right);
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpEqu *deep_copy() const override;
    void to_latex(std::ofstream &f) const override;
    void print() const override;
};
