
#include "Expression.hpp"

Expression::Expression(uint8_t type, double dvalue, std::string var, Expression *left, Expression *right, System *sys)
{
	this->type = type;
	this->dval = dvalue;
	this->var = var;
	this->eleft = left;
	this->eright = right;
	this->sys = sys;
}

void Expression::add_arg(Expression *arg)
{
	this->args.push_back(arg);
}

void Expression::print()
{
	switch (this->type)
	{
	case EXPR_TYPE_DOUBLE:
		printf("%f", this->dval);
		break;
	case EXPR_TYPE_VAR:
		printf("%s", this->var.c_str());
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
	case EXPR_TYPE_FUNC:
		printf("%s(\n", this->var.c_str());
		this->sys->print();
		printf(")\n");
		break;
	default:
		fprintf(stderr, "Error: unknown expression type %d\n", this->type), exit(1);
	}
}

void Expression::to_latex(FILE *f, System sys)
{
	switch (this->type)
	{
	case EXPR_TYPE_DOUBLE:
		Latex::double_to_latex(f, this->dval);
		break;
	case EXPR_TYPE_VAR:
		Latex::var_to_latex(f, this->var.c_str());
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
	case EXPR_TYPE_FUNC:
		Latex::var_to_latex(f, this->var.c_str());
		fprintf(f, "\\left(\n");
		for (int i = 0; i < this->args.size(); ++i)
		{
			this->args[i]->to_latex(f, sys);
			if (i != this->args.size() - 1)
				fprintf(f, ", ");
		}
		fprintf(f, "\\right)\n");
		break;
	}
}

static void replace_args(System *mother_sys, const gsl_vector *x, Expression *exp)
{
	bool ok = false;

	switch (exp->type)
	{
	case EXPR_TYPE_DOUBLE:
		break;
	case EXPR_TYPE_VAR:
		exp->type = EXPR_TYPE_DOUBLE;
		for (int i = 0; i < mother_sys->vars.size(); i++)
			if (exp->var == mother_sys->vars[i])
				exp->dval = gsl_vector_get(x, i), ok = true;
		if (!ok)
			fprintf(stderr, "Error: variable %s not found\n", exp->var.c_str()), exit(1);
		exp->var = "";
		break;
	case EXPR_TYPE_ADD:
	case EXPR_TYPE_SUB:
	case EXPR_TYPE_MUL:
	case EXPR_TYPE_DIV:
	case EXPR_TYPE_EXP:
	case EXPR_TYPE_EQU:
		replace_args(mother_sys, x, exp->eleft);
		replace_args(mother_sys, x, exp->eright);
		break;
	case EXPR_TYPE_PAR:
		replace_args(mother_sys, x, exp->eleft);
		break;
	case EXPR_TYPE_FUNC:
		for (int i = 0; i < exp->sys->equs.size(); i++)
			replace_args(mother_sys, x, exp->sys->equs[i]->eleft);
		break;
	default:
		fprintf(stderr, "Error: unknown expression type %d\n", exp->type), exit(1);
	}
}

double Expression::eval_function_call(std::vector<double> args)
{
	if (this->type != EXPR_TYPE_FUNC)
		fprintf(stderr, "Error: expression is not a function\n"), exit(1);

	System *cp_sys = new System();

	for (int i = 0; i < this->sys->size(); ++i)
		cp_sys->add_equ(this->sys->equs[i]->deep_copy());
	cp_sys->load_vars_from_equs();

	int j = 0;
	for (int i = 0; i < cp_sys->equs.size(); ++i)
	{
		if (cp_sys->equs[i]->type != EXPR_TYPE_EQU)
			fprintf(stderr, "Error: expression is not an equation\n"), exit(1);

		if (cp_sys->equs[i]->eleft->type == EXPR_TYPE_VAR && cp_sys->equs[i]->eleft->var.front() == '@')
		{
			std::string var = cp_sys->vars[i];
			var.erase(0, 1);
			cp_sys->add_equ(
				new Expression(
					EXPR_TYPE_EQU, 0, "",
					new Expression(EXPR_TYPE_VAR, 0, var, NULL, NULL, NULL),
					new Expression(EXPR_TYPE_DOUBLE, args[i], "", NULL, NULL, NULL),
					NULL
				)
			);
			++j;
		}
	}

	for (int i = 0; i < j; ++i)
		cp_sys->equs.erase(cp_sys->equs.begin());

	cp_sys->load_vars_from_equs();

	double res[cp_sys->size()];
	cp_sys->solve(res);

	return res[cp_sys->size() - 1];
}

double Expression::eval(System *mother_sys, const gsl_vector *x)
{
	std::vector<double> args;
	int var_index;

    switch (this->type)
    {
    case EXPR_TYPE_DOUBLE:
        return this->dval;
    case EXPR_TYPE_VAR:
		for (int i = 0; i < mother_sys->vars.size(); ++i)
			if (mother_sys->vars[i] == this->var)
				var_index = i;
		if (var_index == -1)
			fprintf(stderr, "Error: variable %s not found\n", this->var.c_str()), exit(1);
        return gsl_vector_get(x, var_index);
    case EXPR_TYPE_ADD:
        return this->eleft->eval(mother_sys, x) + this->eright->eval(mother_sys, x);
    case EXPR_TYPE_SUB:
        return this->eleft->eval(mother_sys, x) - this->eright->eval(mother_sys, x);
    case EXPR_TYPE_MUL:
        return this->eleft->eval(mother_sys, x) * this->eright->eval(mother_sys, x);
    case EXPR_TYPE_DIV:
        return this->eleft->eval(mother_sys, x) / this->eright->eval(mother_sys, x);
    case EXPR_TYPE_EXP:
        return pow(this->eleft->eval(mother_sys, x), this->eright->eval(mother_sys, x));
    case EXPR_TYPE_PAR:
        return this->eleft->eval(mother_sys, x);
    case EXPR_TYPE_EQU:
        return this->eleft->eval(mother_sys, x) - this->eright->eval(mother_sys, x);
	case EXPR_TYPE_FUNC:
		for (int i = 0; i < this->args.size(); ++i)
			args.push_back(this->args[i]->eval(mother_sys, x));
		return this->eval_function_call(args);
    default:
        fprintf(stderr, "Error: unknown expression type %d\n", this->type), exit(1);
    }
}

Expression *Expression::deep_copy()
{
	Expression *exp = new Expression(this->type, this->dval, this->var, NULL, NULL, NULL);

	switch (this->type)
	{
	case EXPR_TYPE_DOUBLE:
		break;
	case EXPR_TYPE_VAR:
		break;
	case EXPR_TYPE_ADD:
	case EXPR_TYPE_SUB:
	case EXPR_TYPE_MUL:
	case EXPR_TYPE_DIV:
	case EXPR_TYPE_EXP:
	case EXPR_TYPE_EQU:
		exp->eleft = this->eleft->deep_copy();
		exp->eright = this->eright->deep_copy();
		break;
	case EXPR_TYPE_PAR:
		exp->eleft = this->eleft->deep_copy();
		break;
	case EXPR_TYPE_FUNC:
		exp->sys = new System();
		for (int i = 0; i < this->sys->size(); ++i)
			exp->sys->add_equ(this->sys->equs[i]->deep_copy());
		break;
	default:
		fprintf(stderr, "Error: unknown expression type %d\n", this->type), exit(1);
	}

	return exp;
}
