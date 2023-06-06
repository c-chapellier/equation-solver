
#include "Latex.hpp"

void Latex::replace_greek_letters(std::string &s)
{
    for (auto greek_letter : Latex::greek_letters)
    {
        std::string const match = greek_letter;
        std::string const replace = std::string("\\") + greek_letter + " ";

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

auto Latex::var_to_latex(std::string var) -> std::string
{
    Latex::replace_greek_letters(var);

    int const i = var.find('_');

    if (i == -1) {
        return var;
    } 
            std::string sub = var.substr(i + 1);
        Latex::replace_all(sub, "_", "\\_");
        return var.substr(0, i) + "_{" + sub + "}";
   
}

auto Latex::double_to_latex(double n) -> std::string
{
    std::string s = std::to_string(n);

    while (s.back() == '0') {
        s.pop_back();
}

    if (s.back() == '.') {
        s.pop_back();
}

    return s;
}
