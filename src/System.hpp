
#pragma once

#include "es.hpp"

class System
{
private:
    static int rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f);
    void print_state(size_t iter, int n, gsl_multiroot_fsolver *s);

public:
    int n_equs, n_vars;
    Expression *equs[100];
    char *vars[100];

public:
    System();
    void add_equ(Expression *equ);
    int add_var(const char *var);
    int solve(double *res);
    void print() const;
    int size() const;
    int save_to_file(const char *fname, double *res);
    int save_to_markdown(const char *fname, double *res); 
};
