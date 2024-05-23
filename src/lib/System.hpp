
#pragma once

#include "es.hpp"

#include "SIUnit.hpp"

class ExpOp;

extern System main_sys;

class System
{
private:
    static int rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f);
    static void print_state(size_t iter, int n, gsl_multiroot_fsolver *s);
    void sort_equs_and_vars();

public:
    std::vector<ExpOp *> equs;
    std::vector<ExpOp *> inferred_equs;
	std::vector<ExpOp *> unknown_equs;
	std::map<std::string, ExpVar *> vars;
    std::map<std::string, ExpVar *> inferred_vars;
	std::map<std::string, ExpVar *> unknown_vars;
	std::vector<ExpVar *> vars_to_delete;

    std::map<std::string, double> default_constants = std::map<std::string, double>({
        { "pi", M_PI },
        { "e", M_E },
    });

public:
    System();
    ~System();

    void add_equ(ExpOp *equ);
    void add_sys(System *sys);

	void add_equs_from_func_calls();
    void singularize_vars();
    void infer();
    int solve();

    size_t size() const;
    System *deep_copy() const;

    void add_prefix_to_vars(std::string prefix);

    friend std::ostream &operator<<(std::ostream &os, const System &sys);
};
