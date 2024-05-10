
#pragma once

#include "es.hpp"

#include "../SIUnit.hpp"

class System;
class ExpVar;

class Exp
{
public:
    bool is_value_known;
    double value;
    SIUnit unit;

public:
    Exp(): is_value_known(false), value(-1), unit() {}
    Exp(double value, std::string unit): is_value_known(true), value(value), unit(unit) {}
    virtual ~Exp() {}
    virtual double eval(System *mother_sys, const gsl_vector *x) const = 0;
    virtual Exp *deep_copy() const = 0;
    virtual std::string to_latex() const = 0;
    virtual void print() const = 0;
    virtual bool is_linear() const = 0;
    virtual bool infer_units(std::vector<ExpVar *> &vars, SIUnit unit, bool is_value_known, double value = -1) = 0;
    virtual Exp *singularize_vars() = 0;
    virtual bool is_completly_infered() const = 0;
};
