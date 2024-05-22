
#include "es.hpp"

int sys_to_file(std::string fname, std::map<std::string, Function *> funcs, system_t sys)
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

int sys_to_markdown(std::string fname, std::map<std::string, Function *> funcs, system_t sys)
{
    std::ofstream f;

    f.open(fname);
    if (!f.is_open())
        return -1;

    f << "# " << fname << std::endl << std::endl;
    
    f << "## Functions" << std::endl << std::endl;
    for (auto it = funcs.begin(); it != funcs.end(); ++it)
    {
        if (it->second->call_count > 0)
        {
            f << "$$";
            f << it->second->to_latex();
            f << "$$" << std::endl << std::endl;
        }
    }

    f << "## Inferred Equations" << std::endl << std::endl;
    for (int i = 0; i < sys.n_inferred_equs; ++i)
    {
        f << "$$";
        f << sys.inferred_equs[i]->to_latex();
        f << "$$" << std::endl << std::endl;
    }

    f << "## System of Equations" << std::endl << std::endl;
    for (int i = 0; i < sys.n_unknown_equs; ++i)
    {
        f << "$$";
        f << sys.unknown_equs[i]->to_latex();
        f << "$$" << std::endl << std::endl;
    }

    f << "## Inferred Solutions" << std::endl << std::endl;
    for (auto &v : sys.inferred_vars)
    {
        f << "$$";
        f << v.second->to_latex();
        f << " = ";
        f << Latex::double_to_latex(v.second->value);
        f << "\\," << v.second->unit.to_latex();
        f << "$$" << std::endl << std::endl;
    }

    f << "## Variables" << std::endl << std::endl;
    for (auto &v : sys.unknown_vars)
    {
        f << "$$";
        f << v.second->to_latex();
        f << " = ";
        f << Latex::double_to_latex(v.second->value);
        f << "\\," << v.second->unit.to_latex();
        f << "$$" << std::endl << std::endl;
    }

    f.close();
    return 0;
}