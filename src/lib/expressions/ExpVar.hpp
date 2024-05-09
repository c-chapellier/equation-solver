
#pragma once

#include "es.hpp"

extern System main_sys;

class ExpVar : public Exp
{
public:
    std::string var;
    double guess;
    bool true_var;
    int index;
    
public:
    ExpVar(std::string var, std::string guess = "{1}");
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpVar *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
    bool is_linear() const override;
    std::vector<ExpVar *> get_vars() override;
    void units_descent(SIUnit unit) override;
    Exp * singularize_vars() override;
};
