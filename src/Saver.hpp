
#pragma once

#include "es.hpp"

class Saver
{
public:
    static int save_to_file(const std::string &fname, const std::map<std::string, Function *> &funcs, const System &sys, const std::vector<double> &res);
    static int save_to_markdown(const std::string &fname, const std::map<std::string, Function *> &funcs, const System &sys, const std::vector<double> &res);
};
