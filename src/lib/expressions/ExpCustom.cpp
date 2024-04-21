
#include "ExpCustom.hpp"

ExpCustom::ExpCustom(int n_args, double (*func)(double *), const char *str_repr, const char *latex_repr)
    : Exp(), n_args(n_args), func(func), str_repr(str_repr), latex_repr(latex_repr)
{

}

void ExpCustom::load_vars_into_sys(System *sys) const
{

}

double ExpCustom::eval(System *mother_sys, const gsl_vector *x) const
{
    if (x->size < this->n_args + 1)
        std::cerr << "ExpCustom::eval: x->size < 2" << std::endl, exit(1);
    double *args = new double[this->n_args];
    for (int i = 0; i < this->n_args; i++)
        args[i] = gsl_vector_get(x, i + 1);
    double res = this->func(args);
    delete[] args;
    return res;
}

ExpCustom *ExpCustom::deep_copy() const
{
    return new ExpCustom(this->n_args, this->func, this->str_repr, this->latex_repr);
}

std::string ExpCustom::to_latex() const
{
    return std::string(this->latex_repr);
}

void ExpCustom::print() const
{
    std::cout << this->str_repr;
}
