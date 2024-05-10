
#include "Saver.hpp"

int Saver::save_to_file(const std::string &fname, const std::map<std::string, Function *> &funcs, const System &sys)
{
    std::ofstream f;

    f.open(fname);
    if (!f.is_open())
        return -1;

    for (auto &v : sys.vars)
        f << v.first << " = " << v.second->value << std::endl;

    f.close();
    return 0;
}

int Saver::save_to_markdown(const std::string &fname, const std::map<std::string, Function *> &funcs, const System &sys)
{
    std::ofstream f;

    f.open(fname);
    if (!f.is_open())
        return -1;

    f << "# " << fname << std::endl << std::endl;
    
    f << "## Functions" << std::endl << std::endl;
    for (auto it = funcs.begin(); it != funcs.end(); ++it)
    {
        if (it->second->been_called)
        {
            f << "$$";
            f << it->second->to_latex();
            f << "$$" << std::endl << std::endl;
        }
    }

    f << "## Inferred Equations" << std::endl << std::endl;
    for (int i = 0; i < sys.inferred_equs.size(); ++i)
    {
        f << "$$";
        f << sys.inferred_equs[i]->to_latex();
        f << "$$" << std::endl << std::endl;
    }

    f << "## System of Equations" << std::endl << std::endl;
    for (int i = 0; i < sys.unknown_equs.size(); ++i)
    {
        f << "$$";
        f << sys.unknown_equs[i]->to_latex();
        f << "$$" << std::endl << std::endl;
    }

    f << "## Inferred Saolutions" << std::endl << std::endl;
    for (auto &v : sys.inferred_vars)
    {
        f << "$$";
        f << v.second->to_latex();
        f << " = ";
        f << Latex::double_to_latex(v.second->value);
        if (!v.second->unit.is_known)
            f << "\\,[?]";
        else
        {
            std::string u = v.second->unit.to_string();
            if (u == "")
                f << "\\,[?]";
            else if (u != "\\")
                f << "\\,[" << u << "]";
        }
        f << "$$" << std::endl << std::endl;
    }

    f << "## Variables" << std::endl << std::endl;
    for (auto &v : sys.unknown_vars)
    {
        f << "$$";
        f << v.second->to_latex();
        f << " = ";
        f << Latex::double_to_latex(v.second->value);
        if (!v.second->unit.is_known)
            f << "\\,[?]";
        else
        {
            std::string u = v.second->unit.to_string();
            if (u == "")
                f << "\\,[?]";
            else if (u != "\\")
                f << "\\,[" << u << "]";
        }
        f << "$$" << std::endl << std::endl;
    }

    f.close();
    return 0;
}