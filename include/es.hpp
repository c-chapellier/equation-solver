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

#include "../src/lib/expressions/Exp.hpp"
#include "../src/lib/expressions/ExpNum.hpp"
#include "../src/lib/expressions/ExpVar.hpp"
#include "../src/lib/expressions/ExpOp.hpp"
#include "../src/lib/expressions/ExpFuncCall.hpp"
#include "../src/lib/expressions/ExpCustom.hpp"

#include "../src/lib/Latex.hpp"

#include "../src/lib/Function.hpp"

#include "../src/lib/Saver.hpp"

#include "../src/lib/System.hpp"

inline std::map<std::string, Function *> funcs;
