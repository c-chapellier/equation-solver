
#pragma once

#include "es.hpp"

class ExpAdd : public Exp
{
private:
    Exp *eleft;
    Exp *eright;

public:
    ExpAdd(Exp *left, Exp *right);
    ~ExpAdd() override;
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpAdd *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
};
