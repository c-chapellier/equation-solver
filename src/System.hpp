
#pragma once

#include "nonlinear_equ_sys_solver.h"

class System
{
private:
    static int rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f);
    void print_state(size_t iter, int n, gsl_multiroot_fsolver *s);

public:
    int n, n_equs, n_vars;
    Expression *equs[100];
    char *vars[100];

public:
    System();
    void add_equ(Expression *equ);
    int register_var(const char *var);
    int solve(double *res);
    void print();
};
