
#pragma once

#include "es.hpp"

class ExpMul : public Exp
{
private:
    Exp *eleft;
    Exp *eright;
    
public:
    ExpMul(Exp *left, Exp *right);
    ~ExpMul() override;
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpMul *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
