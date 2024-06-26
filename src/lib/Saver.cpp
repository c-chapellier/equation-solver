
#include "Saver.hpp"

int Saver::save_to_file(const std::string &fname, const System &sys)
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

int Saver::save_to_markdown(const std::string &fname, const System &sys)
{
    std::ofstream f;

    f.open(fname);
    if (!f.is_open())
        return -1;

    f << "# " << fname << std::endl << std::endl;
    
    bool has_funcs = false;
    for (auto it = funcs.begin(); it != funcs.end(); ++it)
    {
        if (it->second->call_count > 0)
        {
            if (!has_funcs)
            {
                f << "## Functions" << std::endl << std::endl;
                has_funcs = true;
            }
            f << "$$";
            f << it->second->to_latex();
            f << "$$" << std::endl << std::endl;
        }
    }

    if (sys.inferred_equs.size() > 0)
    {
        f << "## Inferred Equations" << std::endl << std::endl;
        for (int i = 0; i < sys.inferred_equs.size(); ++i)
        {
            f << "$$";
            f << sys.inferred_equs[i]->to_latex();
            f << "$$" << std::endl << std::endl;
        }
    }

    if (sys.inferred_vars.size() > 0)
    {
        f << "## Inferred Solutions" << std::endl << std::endl;
        for (auto &v : sys.inferred_vars)
        {
            f << "$$";
            f << v.second->to_latex();
            f << " = ";
            f << Latex::double_to_latex(v.second->value);
            f << v.second->unit.to_latex();
            f << "$$" << std::endl << std::endl;
        }
    }

    if (sys.unknown_equs.size() > 0)
    {
        f << "## System of Equations" << std::endl << std::endl;
        for (int i = 0; i < sys.unknown_equs.size(); ++i)
        {
            f << "$$";
            f << sys.unknown_equs[i]->to_latex();
            f << "$$" << std::endl << std::endl;
        }
    }

    if (sys.unknown_vars.size() > 0)
    {
        f << "## Solutions" << std::endl << std::endl;
        for (auto &v : sys.unknown_vars)
        {
            f << "$$";
            f << v.second->to_latex();
            f << " = ";
            f << Latex::double_to_latex(v.second->value);
            f << v.second->unit.to_latex();
            f << "$$" << std::endl << std::endl;
        }
    }

    f.close();
    return 0;
}
