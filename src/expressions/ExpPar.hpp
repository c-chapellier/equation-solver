
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpPar : public Exp
{
public:
    Exp *eleft;
    
public:
    ExpPar(Exp *left);
    ~ExpPar();
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpPar *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
