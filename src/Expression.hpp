
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

class Expression
{
public:
    uint8_t type;
    double dval;
    int var;
    Expression *eleft;
    Expression *eright;

public:
    Expression(uint8_t type, double dvalue, int var, Expression *left, Expression *right);
    void print();
    void to_latex(FILE *f, System sys);
    double eval(const gsl_vector *x);
};
