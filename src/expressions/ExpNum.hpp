
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpNum : public Exp
{
public:
    ExpNum(double dval);
    double eval(System *mother_sys, const gsl_vector *x);
    void replace_args(System *mother_sys, const gsl_vector *x);
    ExpNum *deep_copy();
    void to_latex(FILE *f);
    void print();
};
