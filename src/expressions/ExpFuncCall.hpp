
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpFuncCall : public Exp
{
public:
    std::string var;
    System *sys;
    std::vector<Exp *> args;
    
public:
    ExpFuncCall(std::string var, System *sys);
    double eval(System *mother_sys, const gsl_vector *x);
    ExpFuncCall *deep_copy();
    void to_latex(FILE *f);
    void print();
    void add_arg(Exp *arg);
};
