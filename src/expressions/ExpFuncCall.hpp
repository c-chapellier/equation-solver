
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpFuncCall : public Exp
{
public:
    ExpFuncCall(std::string var, System *sys);
    double eval(System *mother_sys, const gsl_vector *x);
    void replace_args(System *mother_sys, const gsl_vector *x);
    ExpFuncCall *deep_copy();
    void to_latex(FILE *f);
    void print();
};
