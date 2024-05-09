
#pragma once

#include "es.hpp"

extern System main_sys;

class ExpVar : public Exp
{
public:
    std::string name;
    double guess;
    bool is_true_var;
    int index;
    
public:
    ExpVar(std::string name, std::string guess = "{1}");
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpVar *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
    bool is_linear() const override;
    std::vector<ExpVar *> units_ascent() override;
    void units_descent(SIUnit unit) override;
    Exp * singularize_vars() override;
};
