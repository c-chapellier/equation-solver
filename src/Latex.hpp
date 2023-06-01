
#pragma once

#include "es.hpp"

class Latex
{
private:

    static void replace_all(std::string &data, const std::string &match, const std::string &replace) 
    {
        size_t pos = data.find(match);

        while( pos != std::string::npos)
        {
            data.replace(pos, match.size(), replace);
            pos = data.find(match, pos+replace.size());
        }
    }
 
public:
    static std::string var_to_latex(std::string var)
    {
        int i = var.find('_');

        if (i == -1)
            return var;
        else
        {
            std::string sub = var.substr(i + 1);
            Latex::replace_all(sub, "_", "\\_"); 
            return var.substr(0, i) + "_{" + sub + "}";
        }
    }

    static std::string double_to_latex(double n)
    {
        std::string s = std::to_string(n);

        while (s.back() == '0')
            s.pop_back();

        if (s.back() == '.')
            s.pop_back();

        return s;
    }
};
