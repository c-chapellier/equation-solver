
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpVar : public Exp
{
public:
    double dval;
    std::string var;
    
public:
    ExpVar(std::string var);
    void load_vars_into_sys(System *sys);
    double eval(System *mother_sys, const gsl_vector *x);
    ExpVar *deep_copy();
    void to_latex(FILE *f);
    void print();
};
