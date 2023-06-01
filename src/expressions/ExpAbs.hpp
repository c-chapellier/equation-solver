
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpAbs : public Exp
{
public:
    
public:
    ExpAbs();
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpAbs *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
