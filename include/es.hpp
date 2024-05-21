#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <span>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>

#define DEBUG(x) std::cout << "\033[33m" << __FILE__ << ":" << __LINE__ << "\033[0m: " << x << std::endl

enum OpType
{
	ADD,
	SUB,
	EQU,
	MUL,
	DIV,
	PAR,
	POW
};

// System Interface Unit
typedef struct siu_s
{
    bool is_known;
    enum UnitType { S, M, KG, A, K, MOL, CD };
    int units[7];
} siu_t;


#include "es.hpp"

void siu_init(siu_t *siu);
void siu_init(siu_t *siu, std::string unit);
int siu_str_to_unit(std::string unit);
bool siu_compare(siu_t a, siu_t b);
// std::ostream &operator<<(std::ostream &os, const SIUnit &sys);
std::string siu_to_latex(siu_t siu);
siu_t siu_multiply(siu_t a, siu_t b);
siu_t siu_divide(siu_t a, siu_t b);

class ExpOp;
class ExpVar;

typedef struct System_s
{
    std::vector<ExpOp *> equs;
    std::vector<ExpOp *> inferred_equs;
	std::vector<ExpOp *> unknown_equs;
	std::map<std::string, ExpVar *> vars;
    std::map<std::string, ExpVar *> inferred_vars;
	std::map<std::string, ExpVar *> unknown_vars;
} System ;

extern System main_sys;

void sys_free(System *sys);
size_t sys_size(System *sys);
void sys_add_equ(System *sys, ExpOp *equ);
void sys_add_sys(System *sys, System *sys_to_add);
void sys_add_equs_from_func_calls(System *sys);
int sys_rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f);
void sys_print_state(size_t iter, int n, gsl_multiroot_fsolver *s);
int sys_solve(System *sys);
void sys_sort_equs_and_vars(System *sys);
void sys_infer(System *sys);
System *sys_deep_copy(System *sys);
void sys_singularize_vars(System *sys);
void sys_add_prefix_to_vars(System *sys, std::string prefix);

#include "../src/lib/expressions/Exp.hpp"
#include "../src/lib/expressions/ExpNum.hpp"
#include "../src/lib/expressions/ExpVar.hpp"
#include "../src/lib/expressions/ExpOp.hpp"
#include "../src/lib/expressions/ExpFuncCall.hpp"
#include "../src/lib/expressions/ExpCustom.hpp"

#include "../src/lib/Latex.hpp"

#include "../src/lib/Function.hpp"

int sys_to_file(const std::string &fname, const std::map<std::string, Function *> &funcs, const System &sys);
int sys_to_markdown(const std::string &fname, const std::map<std::string, Function *> &funcs, const System &sys);


inline std::map<std::string, Function *> funcs;

struct DefaultFunc
{
	std::string name;
	std::vector<std::string> args;
	double (*func)(double *);
	const char *str_repr;
	const char *latex_repr;
};

// inline DefaultFunc default_funcs[] = {
// 	{ "abs", { "x" }, [](double *args) -> double { return abs(args[0]); }, "\\mid x \\mid", "|x|" },
// 	{ "sin", { "x" }, [](double *args) -> double { return sin(args[0]); }, "\\sin(x)", "\\sin(x)" },
// 	{ "cos", { "x" }, [](double *args) -> double { return cos(args[0]); }, "\\cos(x)", "\\cos(x)" },
// 	{ "tan", { "x" }, [](double *args) -> double { return tan(args[0]); }, "\\tan(x)", "\\tan(x)" },
// 	{ "exp", { "x" }, [](double *args) -> double { return exp(args[0]); }, "\\exp(x)", "e^{x}" },
// 	{ "log", { "x" }, [](double *args) -> double { return log(args[0]); }, "\\log(x)", "\\log(x)" },
// 	{ "sqrt", { "x" }, [](double *args) -> double { return sqrt(args[0]); }, "\\sqrt{x}", "\\sqrt{x}" },
// 	{ "atan2", { "x", "y" }, [](double *args) -> double { return atan2(args[0], args[1]); }, "\\text{atan2}(x, y)", "\\text{atan2}(x, y)" },
// 	{ "hypot", { "x", "y" }, [](double *args) -> double { return hypot(args[0], args[1]); }, "\\text{hypot}(x, y)", "\\text{hypot}(x, y)" },
// 	{ "min", { "a", "b" }, [](double *args) -> double { return std::min(args[0], args[1]); }, "min(x, y)", "\\min(x, y)" },
// 	{ "max", { "a", "b" }, [](double *args) -> double { return std::max(args[0], args[1]); }, "max(x, y)", "\\max(x, y)" }
// };
