
#pragma once

#include "es.hpp"

class System
{
private:
    static int rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f);
    void print_state(size_t iter, int n, gsl_multiroot_fsolver *s);
    void add_var(std::string var);

public:
    std::vector<Expression *> equs;
    std::vector<std::string> vars;
    int sys_num;

public:
    System(int sys_num);
    void add_equ(Expression *equ);
    void add_sys(System *sys);
    int solve(double *res);
    void load_vars_from_exp(Expression *exp);
    void load_vars_from_equs();
    void print() const;
    int size() const;
    int save_to_file(const std::string &fname, double *res) const;
    int save_to_markdown(const std::string &fname, double *res) const;
};
