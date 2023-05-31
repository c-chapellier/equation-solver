
#pragma once

#include "es.hpp"

#include "Exp.hpp"

class Function;

class ExpFuncCall : public Exp
{
public:
    std::string var;
    System *sys;
    std::vector<Exp *> args;
    Function *f;
    
public:
    ExpFuncCall(Function *f, std::vector<Exp *> *args, System *sys);
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpFuncCall *deep_copy() const override;
    void to_latex(std::ofstream &f) const override;
    void print() const override;
    void add_arg(Exp *arg);
};
