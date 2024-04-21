
#pragma once

#include "es.hpp"

class ExpPar : public Exp
{
private:
    Exp *eleft;
    
public:
    ExpPar(Exp *left);
    ~ExpPar() override;
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpPar *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
