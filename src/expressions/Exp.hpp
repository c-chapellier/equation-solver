
#pragma once

#include "es.hpp"

class System;

class Exp
{
public:
    virtual void load_vars_into_sys(System *sys) const = 0;
    virtual double eval(System *mother_sys, const gsl_vector *x) const = 0;
    virtual Exp *deep_copy() const = 0;
    virtual void to_latex(std::ofstream &f) const = 0;
    virtual void print() const = 0;
};
