
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpAdd : public Exp
{
public:
    Exp *eleft;
    Exp *eright;

public:
    ExpAdd(Exp *left, Exp *right);
    void load_vars_into_sys(System *sys);
    double eval(System *mother_sys, const gsl_vector *x);
    ExpAdd *deep_copy();
    void to_latex(FILE *f);
    void print();
};
