
#pragma once

#include "es.hpp"

#include "../SIUnit.hpp"

class ExpNum : public Exp
{
private:
    double dval;
    
public:
    ExpNum(double dval);
    ExpNum(double dval, std::string unit);
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpNum *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
    bool is_linear() const override;
    bool infer_units(std::vector<ExpVar *> &vars, SIUnit unit) override;
    Exp *singularize_vars() override;
};
