
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpVar : public Exp
{
public:
    ExpVar(std::string var);
    double eval(System *mother_sys, const gsl_vector *x);
    void replace_args(System *mother_sys, const gsl_vector *x);
    ExpVar *deep_copy();
    void to_latex(FILE *f);
    void print();
};
