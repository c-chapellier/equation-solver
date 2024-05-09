
#pragma once

#include "es.hpp"

extern System main_sys;

class ExpVar : public Exp
{
public:
    std::string var;
    double guess;
    
public:
    ExpVar(std::string var, std::string guess = "{1}");
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpVar *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
    bool is_linear() const override;
    std::vector<ExpVar *> get_vars() override;
    void units_descent(SIUnit unit) override;
    Exp * singularize_vars() override;
};
