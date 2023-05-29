
#include "Expression.hpp"

Expression::Expression(uint8_t type, double dvalue, int var, Expression *left, Expression *right)
{
	this->type = type;
	this->dval = dvalue;
	this->var = var;
	this->eleft = left;
	this->eright = right;
}

void Expression::print()
{
	switch (this->type)
	{
	case EXPR_TYPE_DOUBLE:
		printf("%f", this->dval);
		break;
	case EXPR_TYPE_VAR:
		printf("vars[%d]", this->var);
		break;
	case EXPR_TYPE_ADD:
		this->eleft->print();
		printf(" + ");
		this->eright->print();
		break;
	case EXPR_TYPE_SUB:
		this->eleft->print();
		printf(" - ");
		this->eright->print();
		break;
	case EXPR_TYPE_MUL:
		this->eleft->print();
		printf(" * ");
		this->eright->print();
		break;
	case EXPR_TYPE_DIV:
		this->eleft->print();
		printf(" / ");
		this->eright->print();
		break;
	case EXPR_TYPE_EXP:
		this->eleft->print();
		printf(" ^ ");
		this->eright->print();
		break;
	case EXPR_TYPE_PAR:
		printf("(");
		this->eleft->print();
		printf(")");
		break;
	case EXPR_TYPE_EQU:
		this->eleft->print();
		printf(" = ");
		this->eright->print();
		break;
	}
}

static void double_to_latex(FILE *f, double n)
{
	char s[50];
	snprintf(s, 50, "%.10g", n);

	int i = strlen(s);
	while (s[i - 1] == '0')
		i--;
	if (s[i - 1] == '.')
		i--;

	s[i] = '\0';

	fprintf(f, "%s", s);
}

static void var_to_latex(FILE *f, const char *var)
{
	const char *s = strchr(var, '_');
	if (s == NULL)
		fprintf(f, "%s", var);
	else
		fprintf(f, "%.*s_{%s}", (int)(s - var), var, s + 1);
}

void Expression::to_latex(FILE *f, System sys)
{
	switch (this->type)
	{
	case EXPR_TYPE_DOUBLE:
		double_to_latex(f, this->dval);
		break;
	case EXPR_TYPE_VAR:
		var_to_latex(f, sys.vars[this->var]);
		break;
	case EXPR_TYPE_ADD:
		this->eleft->to_latex(f, sys);
		fprintf(f, " + ");
		this->eright->to_latex(f, sys);
		break;
	case EXPR_TYPE_SUB:
		this->eleft->to_latex(f, sys);
		fprintf(f, " - ");
		this->eright->to_latex(f, sys);
		break;
	case EXPR_TYPE_MUL:
		this->eleft->to_latex(f, sys);
		fprintf(f, " \\cdot ");
		this->eright->to_latex(f, sys);
		break;
	case EXPR_TYPE_DIV:
		fprintf(f, "\\frac{");
		this->eleft->to_latex(f, sys);
		fprintf(f, "}{");
		this->eright->to_latex(f, sys);
		fprintf(f, "}");
		break;
	case EXPR_TYPE_EXP:
		this->eleft->to_latex(f, sys);
		fprintf(f, "^{");
		this->eright->to_latex(f, sys);
		fprintf(f, "}");
		break;
	case EXPR_TYPE_PAR:
		fprintf(f, "\\left(");
		this->eleft->to_latex(f, sys);
		fprintf(f, "\\right)");
		break;
	case EXPR_TYPE_EQU:
		this->eleft->to_latex(f, sys);
		fprintf(f, " = ");
		this->eright->to_latex(f, sys);
		break;
	}
}

double Expression::eval(const gsl_vector *x)
{
    switch (this->type)
    {
    case EXPR_TYPE_DOUBLE:
        return this->dval;
    case EXPR_TYPE_VAR:
        return gsl_vector_get(x, this->var);
    case EXPR_TYPE_ADD:
        return this->eleft->eval(x) + this->eright->eval(x);
    case EXPR_TYPE_SUB:
        return this->eleft->eval(x) - this->eright->eval(x);
    case EXPR_TYPE_MUL:
        return this->eleft->eval(x) * this->eright->eval(x);
    case EXPR_TYPE_DIV:
        return this->eleft->eval(x) / this->eright->eval(x);
    case EXPR_TYPE_EXP:
        return pow(this->eleft->eval(x), this->eright->eval(x));
    case EXPR_TYPE_PAR:
        return this->eleft->eval(x);
    case EXPR_TYPE_EQU:
        return this->eleft->eval(x) - this->eright->eval(x);
    default:
        fprintf(stderr, "Error: unknown expression type %d\n", this->type), exit(1);
    }
}
