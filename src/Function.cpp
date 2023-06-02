
#include "Function.hpp"

Function::Function(std::string name, std::vector<std::string> *args_names, System* sys, Exp* exp)
{
    this->name = name;
    this->args_names = args_names;
    this->sys = sys;
    this->exp = exp;
}

Function::~Function()
{
    delete this->args_names;
    delete this->sys;
    delete this->exp;
}
