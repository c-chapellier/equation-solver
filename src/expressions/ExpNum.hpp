
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpNum : public Exp
{
public:
    double dval;
    
public:
    ExpNum(double dval);
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpNum *deep_copy() const override;
    void to_latex(std::ofstream &f) const override;
    void print() const override;
};
