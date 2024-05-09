
#pragma once

#include "es.hpp"

class ExpPar : public Exp
{
private:
    Exp *eleft;
    
public:
    ExpPar(Exp *left);
    ~ExpPar() override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpPar *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
    bool is_linear() const override;
    std::vector<ExpVar *> get_vars() override;
    void units_descent(SIUnit unit) override;
    Exp * singularize_vars() override;
};
