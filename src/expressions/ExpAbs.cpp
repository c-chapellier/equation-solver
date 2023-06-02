
#include "ExpAbs.hpp"

ExpAbs::ExpAbs() : Exp()
{

}

void ExpAbs::load_vars_into_sys(System *sys) const
{

}

double ExpAbs::eval(System *mother_sys, const gsl_vector *x) const
{
    if (x->size < 2)
        std::cerr << "ExpAbs::eval: x->size < 2" << std::endl, exit(1);
    return abs(gsl_vector_get(x, 1));
}

ExpAbs *ExpAbs::deep_copy() const
{
    return new ExpAbs();
}

std::string ExpAbs::to_latex() const
{
    return std::string("\\mid x \\mid");
}

void ExpAbs::print() const
{
    std::cout << "|x|";
}
