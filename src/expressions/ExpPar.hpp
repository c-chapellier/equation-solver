
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpPar : public Exp
{
public:
    ExpPar(Exp *left);
    double eval(System *mother_sys, const gsl_vector *x);
    void replace_args(System *mother_sys, const gsl_vector *x);
    ExpPar *deep_copy();
    void to_latex(FILE *f);
    void print();
};
