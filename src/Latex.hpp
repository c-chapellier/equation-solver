
#pragma once

#include "es.hpp"

class Latex
{
public:
    static std::string var_to_latex(std::string var)
    {
        int i = var.find('_');

        if (i == -1)
            return var;
        else
            return var.substr(0, i) + "_{" + var.substr(i + 1) + "}";
    }

    static std::string double_to_latex(double n)
    {
        return std::to_string(n);
    }
};
