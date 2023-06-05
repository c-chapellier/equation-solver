
#pragma once

#include "es.hpp"

class System
{
private:
    static int rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f);
    static void print_state(size_t iter, int n, gsl_multiroot_fsolver *s);
    void load_vars_from_exp(Exp *exp);

public:
    std::vector<Exp *> equs;
    std::vector<std::string> vars;

public:
    System();
    ~System();
    void add_equ(Exp *equ);
    void add_var(const std::string &var);
    void add_sys(System *sys);
    int size() const;
    void load_vars_from_equs();
    int solve(std::vector<double> &res, std::vector<double> &guesses);
    void print() const;
    System *deep_copy() const;
};
