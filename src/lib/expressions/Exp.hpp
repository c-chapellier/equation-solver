
#pragma once

#include "es.hpp"

#include "../SIUnit.hpp"

class ExpVar;
class ExpNum;
class ExpOp;
class ExpFuncCall;
class ExpCustom;

class Exp
{
public:
    bool is_value_known;
    double value;
    SIUnit unit;

public:
    Exp(): is_value_known(false), value(-1), unit() {}
    Exp(double value, std::string unit): is_value_known(true), value(value), unit(unit) {}
    virtual ~Exp() {}
    virtual double eval(system_t *mother_sys, const gsl_vector *x) const = 0;
    virtual Exp *deep_copy() const = 0;
    virtual std::string to_latex() const = 0;
    virtual bool is_linear() const = 0;
<<<<<<< HEAD
    virtual bool infer_units(std::vector<ExpVar *> &vars, siu_t unit, bool is_value_known, double value = -1) = 0;
    virtual Exp *singularize_vars(system_t *sys) = 0;
=======
    virtual bool infer_units(std::vector<ExpVar *> &vars, SIUnit unit, bool is_value_known, double value = -1) = 0;
    virtual Exp *singularize_vars(System *sys) = 0;
>>>>>>> parent of 4cc2554 (rm class unit)
    virtual bool is_completly_infered() const = 0;
    virtual std::ostream &output(std::ostream &os) const = 0;
	virtual void add_equs_from_func_calls(system_t *sys) = 0;
    virtual void add_prefix_to_vars(std::string prefix) = 0;
};

inline std::ostream &operator<<(std::ostream &os, const Exp &exp)
{
    return exp.output(os);
}
