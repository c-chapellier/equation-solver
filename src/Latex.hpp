
#pragma once

#include "es.hpp"

class Latex
{
public:
    static void var_to_latex(std::ofstream &f, std::string var)
    {
        int i = var.find('_');

        if (i == -1)
            f << var;
        else
            f << var.substr(0, i) << "_{" << var.substr(i + 1) << "}";
    }

    static void double_to_latex(std::ofstream &f, double n)
    {
        f << n;
    }
};
