
#pragma once

#include "es.hpp"

#include "../SIUnit.hpp"

class System;
class ExpVar;

class Exp
{
public:
    SIUnit unit;

public:
    Exp(): unit() {}
    Exp(std::string unit): unit(unit) {}
    virtual ~Exp() {}
    virtual double eval(System *mother_sys, const gsl_vector *x) const = 0;
    virtual Exp *deep_copy() const = 0;
    virtual std::string to_latex() const = 0;
    virtual void print() const = 0;
    virtual bool is_linear() const = 0;
    virtual std::vector<ExpVar *> units_ascent() = 0;
    virtual void units_descent(SIUnit unit) = 0;
    virtual Exp *singularize_vars() = 0;
};
