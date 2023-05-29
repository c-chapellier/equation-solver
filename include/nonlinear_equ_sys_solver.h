
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>

#include "debug.h"

class Expression;
class System;

#include "../src/System.hpp"

#include "../src/Expression.hpp"

int nonlinear_equ_sys_solver(System sys, double *res);
