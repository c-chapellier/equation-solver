
#pragma once

#include "es.hpp"

class ExpCustom : public Exp
{
public:
    int n_args;
    double (*func)(double *);
    const char *str_repr;
    const char *latex_repr;
    
public:
    ExpCustom(int n_args, double (*func)(double *), const char *str_repr, const char *latex_repr);
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpCustom *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
    bool is_linear() const override;
    std::vector<ExpVar *> get_vars() override;
    void units_descent(SIUnit unit) override;
    Exp *singularize_vars() override;
};
