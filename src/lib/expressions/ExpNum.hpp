
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
    std::vector<ExpVar *> get_vars() override;
    void units_descent(SIUnit unit) override;
    Exp *singularize_vars() override;
};
