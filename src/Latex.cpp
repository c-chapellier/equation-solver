
#include "Latex.hpp"

void Latex::replace_greek_letters(std::string &s)
{
    for (const std::string_view &match : Latex::greek_letters)
    {
        std::string replace = std::string("\\") + match.data() + " ";

        size_t pos = s.find(match);

        while (pos != std::string::npos)
        {
            s.replace(pos, match.size(), replace);
            pos = s.find(match, pos + replace.size());
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
    std::string s = std::to_string(n);

    while (s.back() == '0')
        s.pop_back();

    if (s.back() == '.')
        s.pop_back();

    return s;
}
