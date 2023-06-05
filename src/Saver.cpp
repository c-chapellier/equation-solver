
#include "Saver.hpp"

auto Saver::save_to_file(const std::string &fname, const std::map<std::string, Function *> & /*funcs*/, const System &sys, const std::vector<double> &res) -> int
{
    std::ofstream f;

    f.open(fname);
    if (!f.is_open()) {
        return -1;
}

    for (int i = 0; i < sys.vars.size(); ++i) {
        f << sys.vars[i] << " = " << res[i] << std::endl;
}

    f.close();
    return 0;
}

auto Saver::save_to_markdown(const std::string &fname, const std::map<std::string, Function *> &funcs, const System &sys, const std::vector<double> &res) -> int
{
    std::ofstream f;

    f.open(fname);
    if (!f.is_open()) {
        return -1;
}

    f << "# " << fname << std::endl << std::endl;
    
    f << "## Functions" << std::endl << std::endl;
    for (const auto & func : funcs)
    {
        f << "$$";
        f << func.second->to_latex();
        f << "$$" << std::endl << std::endl;
    }

    f << "## Equations" << std::endl << std::endl;
    for (auto equ : sys.equs)
    {
        f << "$$";
        f << equ->to_latex();
        f << "$$" << std::endl << std::endl;
    }

    f << "## Solution" << std::endl << std::endl;
    for (int i = 0; i < sys.vars.size(); ++i)
    {
        f << "$$";
        f << Latex::var_to_latex(sys.vars[i]);
        f << " = ";
        f << Latex::double_to_latex(res[i]);
        f << "$$" << std::endl << std::endl;
    }

    f.close();
    return 0;
}