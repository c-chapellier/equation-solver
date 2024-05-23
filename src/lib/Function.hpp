
#pragma once

#include "es.hpp"

class Function
{
public:
    std::string name;
	std::vector<std::string> args_names;
	System *sys;
	Exp *ret;
	int call_count;

public:
    Function(std::string name, std::vector<std::string> args_names, System *sys, Exp *ret);
	~Function();
	std::string to_latex() const;
	void print() const;
};
