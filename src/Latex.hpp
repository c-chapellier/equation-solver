
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

    static void replace_greek_letters(std::string &s);
    static void replace_all(std::string &data, const std::string &match, const std::string &replace);
 
public:
    static std::string var_to_latex(std::string var);
    static std::string double_to_latex(double n);
};
