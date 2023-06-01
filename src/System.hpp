
#pragma once

#include "es.hpp"

#include "expressions/Exp.hpp"
#include "expressions/ExpEqu.hpp"
#include "expressions/ExpNum.hpp"
#include "expressions/ExpVar.hpp"
#include "expressions/ExpAdd.hpp"
#include "expressions/ExpSub.hpp"
#include "expressions/ExpMul.hpp"
#include "expressions/ExpDiv.hpp"
#include "expressions/ExpExp.hpp"
#include "expressions/ExpPar.hpp"
#include "expressions/ExpFuncCall.hpp"
#include "expressions/ExpAbs.hpp"

#include "Function.hpp"

class System
{
private:
    static int rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f);
    void print_state(size_t iter, int n, gsl_multiroot_fsolver *s);
    void load_vars_from_exp(Exp *exp);

public:
    std::vector<Exp *> equs;
    std::vector<std::string> vars;

public:
    System();
    void add_equ(Exp *equ);
    void add_var(const std::string &var);
    void add_sys(System *sys);
    int size() const;
    void load_vars_from_equs();
    int solve(std::vector<double> &res);
    void print() const;
    System *deep_copy() const;
};
