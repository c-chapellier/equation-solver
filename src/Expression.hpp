
#pragma once

#include "es.hpp"

#define EXPR_TYPE_DOUBLE 0
#define EXPR_TYPE_VAR 1
#define EXPR_TYPE_ADD 2
#define EXPR_TYPE_SUB 3
#define EXPR_TYPE_MUL 4
#define EXPR_TYPE_DIV 5
#define EXPR_TYPE_EXP 6
#define EXPR_TYPE_PAR 7
#define EXPR_TYPE_EQU 8
#define EXPR_TYPE_FUNC 9

class Expression
{
public:
    uint8_t type;
    double dval;
    std::string var;
    Expression *eleft;
    Expression *eright;
    System *sys;
    std::vector<Expression *> args;

public:
    Expression(uint8_t type, double dvalue, std::string var, Expression *left, Expression *right, System *sys);
    void print();
    void to_latex(FILE *f, System sys);
    double eval(System *mother_sys, const gsl_vector *x);
    double eval_function_call(std::vector<double> args);
    void add_arg(Expression *arg);
    Expression *deep_copy();
};
