
#pragma once

#include "es.hpp"

class ExpEqu : public Exp
{
public:
    Exp *eleft;
    Exp *eright;

public:
    ExpEqu(Exp *left, Exp *right);
    ~ExpEqu() override;
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpEqu *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
    bool is_linear() const override;
    std::vector<ExpVar *> get_vars() override;
    void units_descent(SIUnit unit) override;
    Exp *singularize_vars() override;
    Exp *get_left();
};
