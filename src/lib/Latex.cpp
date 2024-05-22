
#include "Latex.hpp"

void Latex::replace_greek_letters(std::string &s)
{
    for (const std::string_view &match : Latex::GREEK_LETTERS)
    {
        std::string replacement = std::string("\\") + match.data() + " ";
        size_t pos = s.find(match);

        while (pos != std::string::npos)
        {
            if ((pos == 0 || s[pos - 1] == '_') && (!std::isalnum(s[pos + match.size()]) || pos + match.size() == s.size()))
                s.replace(pos, match.size(), replacement);
            pos = s.find(match, pos + replacement.size());
        }
    }
}

void Latex::replace_all(std::string &data, const std::string &match, const std::string &replace)
{
    size_t pos = data.find(match);

    while (pos != std::string::npos)
    {
        data.replace(pos, match.size(), replace);
        pos = data.find(match, pos + replace.size());
    }
}

std::string Latex::var_to_latex(std::string var)
{
    Latex::replace_greek_letters(var);

    size_t i = var.find('_');

    if (i == -1)
        return var;
    else
    {
        std::string sub = var.substr(i + 1);
        Latex::replace_all(sub, "_", "\\_");
        return var.substr(0, i) + "_{" + sub + "}";
    }
}

std::string Latex::double_to_latex(double n)
{
    std::stringstream ss;

    if (0.0001 > n && n > -0.0001 || 10000 < n || n < -10000)
        ss << std::scientific << std::setprecision(Latex::SIGNIFICANT_DIGITS - 1) << n;
    else
        ss << std::setprecision(Latex::SIGNIFICANT_DIGITS) << n;

    return ss.str();
}
