
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
    bool is_linear() const override;
    bool infer_units(std::vector<ExpVar *> &vars, SIUnit unit, bool is_value_known, double value = -1) override;
    Exp * singularize_vars() override;
    bool is_completly_infered() const override;
    std::ostream &output(std::ostream &os) const override;
};
