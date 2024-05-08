#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <span>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>

template<typename T>
constexpr void debug(const T &x)
{
    if (1) std::cerr << x << std::endl;
}

enum OpType
{
	ADD,
	SUB,
	MUL,
	DIV,
	EXP,
	EQU
};

#include "../src/lib/expressions/Exp.hpp"
#include "../src/lib/expressions/ExpNum.hpp"
#include "../src/lib/expressions/ExpVar.hpp"
#include "../src/lib/expressions/ExpOp.hpp"
#include "../src/lib/expressions/ExpPar.hpp"
#include "../src/lib/expressions/ExpFuncCall.hpp"
#include "../src/lib/expressions/ExpCustom.hpp"

#include "../src/lib/Latex.hpp"

#include "../src/lib/Function.hpp"

#include "../src/lib/Saver.hpp"

#include "../src/lib/System.hpp"

inline std::map<std::string, Function *> funcs;

struct DefaultFunc
{
	std::string name;
	std::vector<std::string> args;
	double (*func)(double *);
	const char *str_repr;
	const char *latex_repr;
};

inline DefaultFunc default_funcs[] = {
	{ "abs", { "x" }, [](double *args) -> double { return abs(args[0]); }, "\\mid x \\mid", "|x|" },
	{ "sin", { "x" }, [](double *args) -> double { return sin(args[0]); }, "\\sin(x)", "\\sin(x)" },
	{ "cos", { "x" }, [](double *args) -> double { return cos(args[0]); }, "\\cos(x)", "\\cos(x)" },
	{ "tan", { "x" }, [](double *args) -> double { return tan(args[0]); }, "\\tan(x)", "\\tan(x)" },
	{ "exp", { "x" }, [](double *args) -> double { return exp(args[0]); }, "\\exp(x)", "e^{x}" },
	{ "log", { "x" }, [](double *args) -> double { return log(args[0]); }, "\\log(x)", "\\log(x)" },
	{ "sqrt", { "x" }, [](double *args) -> double { return sqrt(args[0]); }, "\\sqrt{x}", "\\sqrt{x}" },
	{ "atan2", { "x", "y" }, [](double *args) -> double { return atan2(args[0], args[1]); }, "\\text{atan2}(x, y)", "\\text{atan2}(x, y)" },
	{ "hypot", { "x", "y" }, [](double *args) -> double { return hypot(args[0], args[1]); }, "\\text{hypot}(x, y)", "\\text{hypot}(x, y)" },
	{ "min", { "a", "b" }, [](double *args) -> double { return std::min(args[0], args[1]); }, "min(x, y)", "\\min(x, y)" },
	{ "max", { "a", "b" }, [](double *args) -> double { return std::max(args[0], args[1]); }, "max(x, y)", "\\max(x, y)" }
};
