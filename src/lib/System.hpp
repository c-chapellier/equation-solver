
#pragma once

#include "es.hpp"

#include "SIUnit.hpp"

class ExpOp;

extern System main_sys;

typedef struct var_s
{
    std::string name;
    double guess;
    SIUnit unit;
    bool true_var;
} var_t;

class System
{
private:
    static int rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f);
    static void print_state(size_t iter, int n, gsl_multiroot_fsolver *s);

public:
    std::vector<ExpOp *> equs;
	std::map<std::string, ExpVar *> vars;

public:
    System();
    ~System();
    void add_equ(ExpOp *equ);
    void add_sys(System *sys);
    size_t size() const;
    void infer_units();
    int solve(std::vector<double> &res, std::vector<double> &guesses);
    void print() const;
    System *deep_copy() const;
    void singularize_vars();
};
