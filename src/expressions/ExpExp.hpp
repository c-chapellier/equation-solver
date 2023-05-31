
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpExp : public Exp
{
public:
    ExpExp(Exp *left, Exp *right);
    double eval(System *mother_sys, const gsl_vector *x);
    void replace_args(System *mother_sys, const gsl_vector *x);
    ExpExp *deep_copy();
    void to_latex(FILE *f);
    void print();
};
