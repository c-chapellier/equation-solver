
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

class System
{
private:
    static int rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f);
    void print_state(size_t iter, int n, gsl_multiroot_fsolver *s);
    void add_var(std::string var);
    void load_vars_from_exp(Exp *exp);

public:
    std::vector<Exp *> equs;
    std::vector<std::string> vars;

public:
    System();
    void add_equ(Exp *equ);
    void add_sys(System *sys);
    int size() const;
    void load_vars_from_equs();
    int solve(double *res);
    void print() const;
    int save_to_file(const std::string &fname, double *res) const;
    int save_to_markdown(const std::string &fname, double *res) const;
};
