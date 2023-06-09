
#pragma once

#include "es.hpp"

class ExpExp : public Exp
{
private:
    Exp *eleft;
    Exp *eright;
    
public:
    ExpExp(Exp *left, Exp *right);
    ~ExpExp() override;
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpExp *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
