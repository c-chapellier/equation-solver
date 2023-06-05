
#pragma once

#include "es.hpp"

class Function;

class ExpFuncCall : public Exp
{
public:
    std::string var;
    System *sys{nullptr};
    std::vector<Exp *> args;
    Function *f{nullptr};
    
public:
    ExpFuncCall();
    ExpFuncCall(Function *f, std::vector<Exp *> &args);
    ~ExpFuncCall();
    void load_vars_into_sys(System *sys) const override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpFuncCall *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
    void add_arg(Exp *arg);
};
