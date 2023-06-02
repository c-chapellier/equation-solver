
#pragma once

#include "es.hpp"

class System;

class Exp
{
public:
    virtual ~Exp() {}
    virtual void load_vars_into_sys(System *sys) const = 0;
    virtual double eval(System *mother_sys, const gsl_vector *x) const = 0;
    virtual Exp *deep_copy() const = 0;
    virtual std::string to_latex() const = 0;
    virtual void print() const = 0;
};
