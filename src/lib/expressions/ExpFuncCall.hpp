
#pragma once

#include "es.hpp"

class Function;

class ExpFuncCall : public Exp
{
public:
    System *sys;
    std::vector<Exp *> args;
    Function *f;
    
public:
    ExpFuncCall();
    ExpFuncCall(Function *f, std::vector<Exp *> &args);
    ~ExpFuncCall() override;
    double eval(System *mother_sys, const gsl_vector *x) const override;
    ExpFuncCall *deep_copy() const override;
    std::string to_latex() const override;
    void print() const override;
    bool is_linear() const override;
    std::vector<ExpVar *> units_ascent() override;
    void units_descent(SIUnit unit) override;
    Exp *singularize_vars() override;
    void add_arg(Exp *arg);
};
