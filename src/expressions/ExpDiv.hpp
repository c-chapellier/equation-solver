
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class ExpDiv : public Exp
{
public:
    Exp *eleft;
    Exp *eright;

public:
    ExpDiv(Exp *left, Exp *right);
    double eval(System *mother_sys, const gsl_vector *x);
    ExpDiv *deep_copy();
    void to_latex(FILE *f);
    void print();
};
