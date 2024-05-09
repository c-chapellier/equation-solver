
#pragma once

#include "es.hpp"

#include "SIUnit.hpp"

extern System main_sys;

class ExpEqu;

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
    void print_state(size_t iter, int n, gsl_multiroot_fsolver *s);
    void load_vars_from_exp(Exp *exp);

public:
    std::vector<ExpEqu *> equs;
	std::map<std::string, ExpVar *> vars;
    std::vector<double> guesses;

public:
    System();
    ~System();
    void add_equ(ExpEqu *equ);
    void add_var(const std::string &name, double guess, SIUnit unit);
    void add_sys(System *sys);
    size_t size() const;
    void load_vars_from_equs();
    int solve(std::vector<double> &res, std::vector<double> &guesses);
    void print() const;
    System *deep_copy() const;
    void singularize_vars();
};
