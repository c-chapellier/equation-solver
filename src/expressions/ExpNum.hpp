
#pragma once

#include "es.hpp"

class ExpNum : public Exp
{
private:
    double dval;
    
public:
    ExpNum(double dval);
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpNum *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
