
#pragma once

#include "es.hpp"

class ExpOp : public Exp
{
public:
    OpType op;
    Exp *eleft;
    Exp *eright;

public:
    ExpOp(OpType op, Exp *left, Exp *right);
    ~ExpOp() override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpOp *deep_copy() const override;
    std::string to_latex() const override;
    bool is_linear() const override;
    bool infer_units(std::vector<ExpVar *> &vars, SIUnit unit, bool is_value_known, double value = -1) override;
    Exp *singularize_vars(System *sys) override;
    bool is_completly_infered() const override;
    void add_equs_from_func_calls(System *sys) override;
    void add_prefix_to_vars(std::string prefix) override;

    std::ostream &output(std::ostream &os) const override;
};
