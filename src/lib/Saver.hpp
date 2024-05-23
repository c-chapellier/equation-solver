
#pragma once

#include "es.hpp"

class Saver
{
public:
    static int save_to_file(const std::string &fname, const System &sys);
    static int save_to_markdown(const std::string &fname, const System &sys);
};
