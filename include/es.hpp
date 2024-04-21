
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
    if (0) std::cerr << x << std::endl;
}

#include "../src/Latex.hpp"

#include "../src/expressions/Exp.hpp"
#include "../src/expressions/ExpEqu.hpp"
#include "../src/expressions/ExpNum.hpp"
#include "../src/expressions/ExpVar.hpp"
#include "../src/expressions/ExpAdd.hpp"
#include "../src/expressions/ExpSub.hpp"
#include "../src/expressions/ExpMul.hpp"
#include "../src/expressions/ExpDiv.hpp"
#include "../src/expressions/ExpExp.hpp"
#include "../src/expressions/ExpPar.hpp"
#include "../src/expressions/ExpFuncCall.hpp"
#include "../src/expressions/ExpCustom.hpp"

#include "../src/Function.hpp"

#include "../src/System.hpp"

#include "../src/Saver.hpp"
