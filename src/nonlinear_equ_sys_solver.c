
#include "nonlinear_equ_sys_solver.h"

// max ((a - b) / (2*fabs(a - b)) + 0.5) * a + ((b - a) / (2*fabs(a - b)) + 0.5) * b

static double eval_expr(const gsl_vector *x, expression_t *expr)
{
    switch (expr->type)
    {
    case EXPR_TYPE_DOUBLE:
        return expr->dval;
    case EXPR_TYPE_VAR:
        return gsl_vector_get(x, expr->var);
    case EXPR_TYPE_ADD:
        return eval_expr(x, expr->eleft) + eval_expr(x, expr->eright);
    case EXPR_TYPE_SUB:
        return eval_expr(x, expr->eleft) - eval_expr(x, expr->eright);
    case EXPR_TYPE_MUL:
        return eval_expr(x, expr->eleft) * eval_expr(x, expr->eright);
    case EXPR_TYPE_DIV:
        return eval_expr(x, expr->eleft) / eval_expr(x, expr->eright);
    case EXPR_TYPE_EXP:
        return pow(eval_expr(x, expr->eleft), eval_expr(x, expr->eright));
    case EXPR_TYPE_PAR:
        return eval_expr(x, expr->eleft);
    default:
        fprintf(stderr, "Error: unknown expression type %d\n", expr->type), exit(1);
    }
}

static double eval_equ(const gsl_vector *x, equation_t *equ)
{
    return eval_expr(x, equ->lvalue) - eval_expr(x, equ->rvalue);
}

static int rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f)
{
    system_t *sys = (system_t *)params;

    double y[sys->n];
    for (int i = 0; i < sys->n; ++i)
        y[i] = eval_equ(x, sys->equs[i]);

    for (int i = 0; i < sys->n; ++i)
        gsl_vector_set(f, i, y[i]);

    return GSL_SUCCESS;
}

static void print_state(size_t iter, int n, gsl_multiroot_fsolver *s)
{
    printf("iter %3zu x = ", iter);
    for (int i = 0; i < n; i++)
        printf("% .3f ", gsl_vector_get(s->x, i));
    printf("f(x) = ");
    for (int i = 0; i < n; i++)
        printf("% .3e ", gsl_vector_get(s->f, i));
    printf("\n");
}

int nonlinear_equ_sys_solver(system_t sys, double *res)
{    
    const gsl_multiroot_fsolver_type *T = gsl_multiroot_fsolver_hybrids;
    gsl_multiroot_fsolver *s = gsl_multiroot_fsolver_alloc(T, sys.n);
    gsl_multiroot_function f = { &rosenbrock_f, sys.n, &sys };
    
    gsl_vector *x = gsl_vector_alloc(sys.n);
    for (int i = 0; i < sys.n; i++)
        gsl_vector_set(x, i, 0.0); // 3rd arg is initial guess

    gsl_multiroot_fsolver_set(s, &f, x);

    int status = GSL_CONTINUE;
    size_t iter = 0;

    if (DEBUG_MODE) print_state(iter, sys.n, s);
    while (status == GSL_CONTINUE && iter++ < 1000)
    {
        status = gsl_multiroot_fsolver_iterate(s);
        if (DEBUG_MODE) print_state(iter, sys.n, s);
        if (status) /* check if solver is stuck */
            break;
        status = gsl_multiroot_test_residual(s->f, 1e-7);
    }

    debug("status = %s\n", gsl_strerror(status));

    for (int i = 0; i < sys.n; i++)
        res[i] = gsl_vector_get(s->x, i);

    gsl_multiroot_fsolver_free(s);
    gsl_vector_free(x);
    return 0;
}