
#pragma once

#include "es.hpp"

class Latex
{
public:
    static void var_to_latex(FILE *f, const char *var)
    {
        const char *s = strchr(var, '_');
        if (s == NULL)
            fprintf(f, "%s", var);
        else
            fprintf(f, "%.*s_{%s}", (int)(s - var), var, s + 1);
    }

    static void double_to_latex(FILE *f, double n)
    {
        char s[50];
        snprintf(s, 50, "%f", n);

        int i = strlen(s);
        while (s[i - 1] == '0')
            i--;
        if (s[i - 1] == '.')
            i--;

        s[i] = '\0';

        fprintf(f, "%s", s);
    }
};
