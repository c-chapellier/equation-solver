
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpVar : public Exp
{
public:
    double dval;
    std::string var;
    
public:
    ExpVar(std::string var);
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpVar *deep_copy() const override;
    void to_latex(std::ofstream &f) const override;
    void print() const override;
};
