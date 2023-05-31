
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpEqu : public Exp
{
public:
    ExpEqu(Exp *left, Exp *right);
    double eval(System *mother_sys, const gsl_vector *x);
    void replace_args(System *mother_sys, const gsl_vector *x);
    ExpEqu *deep_copy();
    void to_latex(FILE *f);
    void print();
};
