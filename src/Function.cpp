
#include "Function.hpp"

<<<<<<< HEAD
#include <utility>


Function::Function(std::string name, std::vector<std::string> &args_names, System *sys, Exp *exp) : name(std::move(std::move(name))), args_names(args_names), sys(sys), exp(exp)
=======
Function::Function(std::string name, std::vector<std::string> &args_names, System *sys, Exp *exp)
>>>>>>> parent of a08e814 (run static analyzer)
{
    this->name = name;
    this->args_names = args_names;
    this->sys = sys;
    this->exp = exp;
}

