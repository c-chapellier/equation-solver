
#include "es.hpp"

#include <sstream>

static const int SIGNIFICANT_DIGITS = 5;
static const int GREEK_LETTERS_SIZE = 68;
static const std::array<std::string_view, GREEK_LETTERS_SIZE> GREEK_LETTERS = {
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

static void latex_replace_greek_letters(std::string &s)
{
    for (const std::string_view &match : GREEK_LETTERS)
    {
        std::string replacement = std::string("\\") + match.data() + " ";
        size_t pos = s.find(match);

        while (pos != std::string::npos)
        {
            if ((pos == 0 || s[pos - 1] == '_') && (!std::isalnum(s[pos + match.size()]) || pos + match.size() == s.size()))
            {
                std::cout << "find match: " << match << " at position: " << pos << "of " << s << std::endl;
                s.replace(pos, match.size(), replacement);
            }
            pos = s.find(match, pos + replacement.size());
        }
    }
}

static void latex_replace_all(std::string &data, const std::string &match, const std::string &replace)
{
    size_t pos = data.find(match);

    while (pos != std::string::npos)
    {
        data.replace(pos, match.size(), replace);
        pos = data.find(match, pos + replace.size());
    }
}

std::string latex_var_to_latex(std::string var)
{
    latex_replace_greek_letters(var);

    size_t i = var.find('_');

    if (i == -1)
        return var;
    else
    {
        std::string sub = var.substr(i + 1);
        latex_replace_all(sub, "_", "\\_");
        return var.substr(0, i) + "_{" + sub + "}";
    }
}

std::string latex_double_to_latex(double n)
{
    std::stringstream ss;

    if (0.0001 > n && n > -0.0001 || 10000 < n || n < -10000)
        ss << std::scientific << std::setprecision(SIGNIFICANT_DIGITS - 1) << n;
    else
        ss << std::setprecision(SIGNIFICANT_DIGITS) << n;

    return ss.str();
}
