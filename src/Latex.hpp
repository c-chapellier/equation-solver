
#pragma once

#include "es.hpp"

class Latex
{
private:
    static constexpr int size = 68;
    static constexpr int max_len = 12;
    static constexpr char greek_letters[size][max_len] = {
        "Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta", "Eta", "Theta",
        "Iota", "Kappa", "Lambda", "Mu", "Nu", "Xi", "Omicron", "Pi",
        "Rho", "Sigma", "Tau", "Upsilon", "Phi", "Chi", "Psi", "Omega",
        "varGamma", "varDelta", "varTheta", "varLambda", "varXi", "varPi",
        "varSigma", "varUpsilon", "varPhi", "varPsi", "varOmega", "alpha",
        "beta", "gamma", "delta", "epsilon", "zeta", "eta", "theta", "iota",
        "kappa", "lambda", "mu", "nu", "xi", "omicron", "pi", "rho", "sigma",
        "tau", "upsilon", "phi", "chi", "psi", "omega", "varepsilon", "varkappa",
        "vartheta", "thetasym", "varpi", "varrho", "varsigma", "varphi", "digamma"
    };

    static void replace_greek_letters(std::string &s) 
    {
        for (int i = 0; i < Latex::size; ++i)
        {
            std::string match = Latex::greek_letters[i];
            std::string replace = std::string("\\") + Latex::greek_letters[i] + " ";

            size_t pos = s.find(match);

            while (pos != std::string::npos)
            {
                s.replace(pos, match.size(), replace);
                pos = s.find(match, pos + replace.size());
            }
        }
    }
    
    static void replace_all(std::string &data, const std::string &match, const std::string &replace) 
    {
        size_t pos = data.find(match);

        while (pos != std::string::npos)
        {
            data.replace(pos, match.size(), replace);
            pos = data.find(match, pos+replace.size());
        }
    }
 
public:
    static std::string var_to_latex(std::string var)
    {
        Latex::replace_greek_letters(var);

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
