
#pragma once

#include "es.hpp"

class System;

class Exp
{
public:
    Exp();
    virtual double eval(System *mother_sys, const gsl_vector *x) = 0;
    virtual Exp *deep_copy() = 0;
    virtual void to_latex(FILE *f) = 0;
    virtual void print() = 0;
};
