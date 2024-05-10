
#pragma once

#include "es.hpp"

extern System main_sys;

class ExpVar : public Exp
{
public:
    std::string name;
    double guess;
    bool can_be_infered;
    int index;
    
public:
    ExpVar(std::string name, std::string guess = "{1}");
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpVar *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
    bool is_linear() const override;
    bool infer_units(std::vector<ExpVar *> &vars, SIUnit unit) override;
    Exp * singularize_vars() override;
};
