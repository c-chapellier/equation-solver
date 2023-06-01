
#pragma once

#include "es.hpp"
#include "expressions/Exp.hpp"

class Function
{
public:
    std::string name;
	std::vector<std::string> *args_names;
	System* sys;
	Exp* exp;

public:
    Function(std::string name, std::vector<std::string> *args_names, System* sys, Exp* exp);
	std::string to_latex() const;
	void print() const;
	Function *deep_copy() const;
};