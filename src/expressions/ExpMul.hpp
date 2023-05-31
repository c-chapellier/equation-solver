
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpMul : public Exp
{
public:
    Exp *eleft;
    Exp *eright;
    
public:
    ExpMul(Exp *left, Exp *right);
    void load_vars_into_sys(System *sys);
    double eval(System *mother_sys, const gsl_vector *x);
    ExpMul *deep_copy();
    void to_latex(FILE *f);
    void print();
};
