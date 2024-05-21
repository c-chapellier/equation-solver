
#pragma once

#include "es.hpp"

class Function
{
public:
    std::string name;
	std::vector<std::string> args_names;
	system_t *sys;
	Exp *ret;
	int call_count;

public:
    Function(std::string name, std::vector<std::string> args_names, system_t *sys, Exp *ret);
	~Function();
	Function *deep_copy() const;
	std::string to_latex() const;
	void print() const;
};
