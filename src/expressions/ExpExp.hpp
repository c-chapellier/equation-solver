
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpExp : public Exp
{
public:
    Exp *eleft;
    Exp *eright;
    
public:
    ExpExp(Exp *left, Exp *right);
    void load_vars_into_sys(System *sys);
    double eval(System *mother_sys, const gsl_vector *x);
    ExpExp *deep_copy();
    void to_latex(FILE *f);
    void print();
};
