
#include "ExpCustom.hpp"

ExpCustom::ExpCustom(int n_args, double (*func)(double *), const char *str_repr, const char *latex_repr)
    : Exp(), n_args(n_args), func(func), str_repr(str_repr), latex_repr(latex_repr)
{

}

double ExpCustom::eval(System *mother_sys, const gsl_vector *x) const
{
    assert(x->size >= this->n_args + 1);

    double *args = new double[this->n_args];

    for (int i = 0; i < this->n_args; i++)
        args[i] = gsl_vector_get(x, i + 1);

    std::cout << "func(" << args[0] << ") = " << this->func(args) << std::endl;
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

bool ExpCustom::is_linear() const
{
   return false; // TODO for some functions
}

bool ExpCustom::infer_units(std::vector<ExpVar *> &vars, SIUnit unit, bool is_value_known, double value)
{
    return true; // TODO for some functions
}

Exp *ExpCustom::singularize_vars(System *sys)
{
    return NULL;
}

bool ExpCustom::is_completly_infered() const
{
    return false; // TODO for some functions
}

std::ostream &ExpCustom::output(std::ostream &os) const
{
    os << this->str_repr;
    return os;
}

void ExpCustom::add_equs_from_func_calls(System *sys)
{
    return ;
}

void ExpCustom::add_prefix_to_vars(std::string prefix)
{
   return ;
}
