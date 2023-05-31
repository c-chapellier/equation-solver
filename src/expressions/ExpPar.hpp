
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpPar : public Exp
{
public:
    Exp *eleft;
    
public:
    ExpPar(Exp *left);
    void load_vars_into_sys(System *sys);
    double eval(System *mother_sys, const gsl_vector *x);
    ExpPar *deep_copy();
    void to_latex(FILE *f);
    void print();
};
