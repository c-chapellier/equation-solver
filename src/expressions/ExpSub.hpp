
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpSub : public Exp
{
public:
    Exp *eleft;
    Exp *eright;
    
public:
    ExpSub(Exp *left, Exp *right);
    double eval(System *mother_sys, const gsl_vector *x);
    ExpSub *deep_copy();
    void to_latex(FILE *f);
    void print();
};
