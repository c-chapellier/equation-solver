
#include "Function.hpp"

Function::Function(std::string name, std::vector<std::string> *args_names, System* sys, Exp* exp)
{
    this->name = name;
    this->args_names = args_names;
    this->sys = sys;
    this->exp = exp;
}

void Function::to_latex(std::ostream &f) const
{
    f << "\\begin{equation}" << std::endl;
    f << "  " << this->name << "(";
    for (int i = 0; i < this->args_names->size(); ++i)
    {
        if (i > 0)
            f << ", ";
        f << this->args_names->at(i);
    }
    f << ") = ";
    this->exp->to_latex(f);
    f << std::endl;
    f << "\\end{equation}" << std::endl;
}