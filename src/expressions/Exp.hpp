
#pragma once

#include "es.hpp"

class System;

class Exp
{
public:
    double dval;
    std::string var;
    Exp *eleft;
    Exp *eright;
    System *sys;
    std::vector<Exp *> args;

public:
    Exp(double dvalue, std::string var, Exp *left, Exp *right, System *sys);
    virtual double eval(System *mother_sys, const gsl_vector *x) = 0;
    virtual void replace_args(System *mother_sys, const gsl_vector *x) = 0;
    virtual Exp *deep_copy() = 0;
    virtual void to_latex(FILE *f) = 0;
    virtual void print() = 0;
    void add_arg(Exp *arg);
};
