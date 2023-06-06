
#pragma once

#include "es.hpp"

class ExpSub : public Exp
{
private:
    Exp *eleft;
    Exp *eright;
    
public:
    ExpSub(Exp *left, Exp *right);
    ~ExpSub() override;
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpSub *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
