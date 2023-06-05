
#include "ExpAbs.hpp"

ExpAbs::ExpAbs() : Exp()
{

}

void ExpAbs::load_vars_into_sys(System *sys) const
{

}

auto ExpAbs::eval(System * /*mother_sys*/, const gsl_vector *x) const -> double
{
    if (x->size < 2) {
        std::cerr << "ExpAbs::eval: x->size < 2" << std::endl, exit(1);
}
    return abs(gsl_vector_get(x, 1));
}

auto ExpAbs::deep_copy() const -> ExpAbs *
{
    return new ExpAbs();
}

auto ExpAbs::to_latex() const -> std::string
{
    return {"\\mid x \\mid"};
}

void ExpAbs::print() const
{
    std::cout << "|x|";
}
