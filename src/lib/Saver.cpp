
#include "Saver.hpp"

int Saver::save_to_file(const std::string &fname, const std::map<std::string, Function *> &funcs, const System &sys, const std::vector<double> &res)
{
    std::ofstream f;

    f.open(fname);
    if (!f.is_open())
        return -1;

    for (auto &v : sys.vars)
        f << v.first << " = " << res[v.second->index] << std::endl;

    f.close();
    return 0;
}

int Saver::save_to_markdown(const std::string &fname, const std::map<std::string, Function *> &funcs, const System &sys, const std::vector<double> &res)
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

    f << "## Equations" << std::endl << std::endl;
    for (int i = 0; i < sys.equs.size(); ++i)
    {
        f << "$$";
        f << sys.equs[i]->to_latex();
        f << "$$" << std::endl << std::endl;
    }

    f << "## Solution" << std::endl << std::endl;
    for (auto &v : sys.vars)
    {
        f << "$$";
        f << v.second->to_latex();
        f << " = ";
        f << Latex::double_to_latex(res[v.second->index]);
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