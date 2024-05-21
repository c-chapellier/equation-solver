
#pragma once

#include "es.hpp"

class ExpNum : public Exp
{
public:
    ExpNum(double value);
    ExpNum(double value, std::string unit);
    ~ExpNum();
    double eval(system_t *mother_sys, const gsl_vector *x) const override;
    ExpNum *deep_copy() const override;
    std::string to_latex() const override;
    bool is_linear() const override;
    bool infer_units(std::vector<ExpVar *> &vars, siu_t unit, bool is_value_known, double value = -1) override;
    Exp *singularize_vars(system_t *sys) override;
    bool is_completly_infered() const override;
    std::ostream &output(std::ostream &os) const override;
    void add_equs_from_func_calls(system_t *sys) override;
    void add_prefix_to_vars(std::string prefix) override;
};
