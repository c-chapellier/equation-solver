%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

#include "es.hpp"

System main_sys = System();

typedef struct
{
	std::string name;
	std::vector<std::string> *args_names;
	System* sys;
	Expression* exp;
} function_t;

function_t *create_function(std::string name, std::vector<std::string> *args_names, System* sys, Expression* exp)
{
	function_t *f = (function_t *)malloc(sizeof (function_t));
	f->name = name;
	f->args_names = args_names;
	f->sys = sys;
	f->exp = exp;
	return f;
}

std::vector<function_t *> functions;

Expression *create_function_call(std::string name, std::vector<Expression *> *args)
{
	function_t *f = NULL;
	for (int i = 0; i < functions.size(); ++i)
		if (functions[i]->name == name)
			f = functions[i];
	
	if (f == NULL)
		yyerror("Function not found");

	if (f->args_names->size() != args->size())
		yyerror("Wrong number of arguments");

	Expression *exp = new Expression(EXPR_TYPE_FUNC, 0, f->name, NULL, NULL, new System());

	for (int i = 0; i < args->size(); ++i)
		exp->args.push_back((*args)[i]);

	for (int i = 0; i < f->args_names->size(); ++i)
	{
		if ((*args)[i]->type == EXPR_TYPE_EQU)
			yyerror("Can't use equation as argument");

		exp->sys->add_equ(
			new Expression(
				EXPR_TYPE_EQU,
				0,
				"",
				new Expression(EXPR_TYPE_VAR, 0, std::string("@") + (*f->args_names)[i], NULL, NULL, NULL),
				(*args)[i],
				NULL
			)
		);
	}

	for (int i = 0; i < f->sys->size(); ++i)
		exp->sys->add_equ(f->sys->equs[i]);

	exp->sys->add_equ(
		new Expression(
			EXPR_TYPE_EQU,
			0,
			"",
			new Expression(EXPR_TYPE_VAR, 0, std::string("#ret"), NULL, NULL, NULL),
			f->exp,
			NULL
		)
	);

	exp->sys->load_vars_from_equs();

	return exp;
}

%}

%union {
	double dval;
	char* sval;
	void *exp_val;
	void *sys_val;
	void *args_def_val;
	void *func_val;
	void *args_val;
}

%token<sval> T_VAR
%token<dval> T_DOUBLE

%token T_EQU T_ADD T_SUB T_MUL T_DIV T_EXP
%token T_LPAR T_RPAR T_LBRA T_RBRA T_COMMA
%token T_RETURN T_FUNC
%token T_NEWLINE T_EOF

%left T_ADD T_SUB
%left T_MUL T_DIV
%left T_EXP

%type<exp_val> exp
%type<exp_val> equ
%type<sys_val> sys
%type<args_def_val> args_def
%type<func_val> func
%type<args_val> args

%start prog

%%

prog:
	| prog block 		{ debug("prog: prog block\n"); }
;

block:
	  func				{ debug("block: func\n"); functions.push_back((function_t *)$1); }
	| sys				{ debug("block: sys\n"); main_sys.add_sys((System *)$1); }

func:
	  T_FUNC T_VAR T_LPAR args_def T_RPAR T_NEWLINE T_LBRA T_NEWLINE sys T_RETURN exp T_NEWLINE T_RBRA
	  { debug("func: \n"); $$ = create_function(std::string($2), (std::vector<std::string> *)$4, (System *)$9, (Expression *)$11); }
;

args_def:
	  T_VAR T_COMMA args_def	{ debug("args_def: T_VAR T_COMMA args_def\n"); ((std::vector<std::string> *)$$)->push_back(std::string($1));}
	| T_VAR						{ debug("args_def: T_VAR\n"); $$ = new std::vector<std::string>(); ((std::vector<std::string> *)$$)->push_back(std::string($1)); }
;

sys:
	  sys equ T_NEWLINE		{ debug("sys: sys equ T_NEWLINE\n"); ((System *)$$)->add_equ((Expression *)$2); }
	| equ T_NEWLINE			{ debug("sys: equ T_NEWLINE\n"); $$ = new System(); ((System *)$$)->add_equ((Expression *)$1);}
	| sys T_NEWLINE			{ debug("sys: sys T_NEWLINE\n");  }
	| sys T_EOF				{ debug("sys: sys T_EOF\n"); }
	| equ T_EOF				{ debug("sys: equ T_EOF\n"); $$ = new System(); ((System *)$$)->add_equ((Expression *)$1);}
	| T_NEWLINE				{ debug("sys: T_NEWLINE\n"); $$ = new System(); }
;

equ: exp T_EQU exp		{ debug("equ: exp T_EQU exp\n"); $$ = new Expression(EXPR_TYPE_EQU, 0, "", (Expression *)$1, (Expression *)$3, NULL); }
;

exp:
	  T_DOUBLE			{ debug("exp: T_DOUBLE\n"); $$ = new Expression(EXPR_TYPE_DOUBLE, $1, "", NULL, NULL, NULL); }
	| T_VAR				{ debug("exp: T_VAR\n"); $$ = new Expression(EXPR_TYPE_VAR, 0, $1, NULL, NULL, NULL); }
	| exp T_ADD exp		{ debug("exp: exp T_ADD exp\n"); $$ = new Expression(EXPR_TYPE_ADD, 0, "", (Expression *)$1, (Expression *)$3, NULL) }
	| exp T_SUB exp		{ debug("exp: exp T_SUB exp\n"); $$ = new Expression(EXPR_TYPE_SUB, 0, "", (Expression *)$1, (Expression *)$3, NULL) }
	| exp T_MUL exp		{ debug("exp: exp T_MUL exp\n"); $$ = new Expression(EXPR_TYPE_MUL, 0, "", (Expression *)$1, (Expression *)$3, NULL) }
	| exp T_DIV exp		{ debug("exp: exp T_DIV exp\n"); $$ = new Expression(EXPR_TYPE_DIV, 0, "", (Expression *)$1, (Expression *)$3, NULL) }
	| exp T_EXP exp		{ debug("exp: exp T_EXP exp\n"); $$ = new Expression(EXPR_TYPE_EXP, 0, "", (Expression *)$1, (Expression *)$3, NULL) }
	| T_LPAR exp T_RPAR	{ debug("exp: T_LPAR exp T_RPA\n"); $$ = new Expression(EXPR_TYPE_PAR, 0, "", (Expression *)$2, NULL, NULL) }
	| T_VAR T_LPAR args T_RPAR	{ debug("exp: T_VAR T_LPAR args T_RPAR\n"); $$ = create_function_call(std::string($1), (std::vector<Expression *> *)$3); }
;

args:
	  exp T_COMMA args	{ debug("args: exp T_COMMA args\n"); ((std::vector<Expression *> *)$$)->push_back((Expression *)$1);}
	| exp				{ debug("args: exp\n"); $$ = new std::vector<Expression *>(); ((std::vector<Expression *> *)$$)->push_back((Expression *)$1); }

%%

int main(int argc, char* argv[])
{
	if (argc != 2)
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]), exit(1);

	yyin = fopen(argv[1], "r");
	if (yyin == NULL)
		fprintf(stderr, "Can't open file %s\n", argv[1]), exit(1);

	do
		yyparse();
	while(!feof(yyin));

	fclose(yyin);

	main_sys.load_vars_from_equs();

	main_sys.print();
	printf("----------- SOLVE ------------\n");

	double res[main_sys.size()];
	main_sys.solve(res);

	debug("Solution:\n");
	for (int i = 0; i < main_sys.size(); ++i)
		debug("  %s = %f\n", main_sys.vars[i].c_str(), res[i]);

	std::string fname = std::string(argv[1]) + ".res";

	if (main_sys.save_to_file(fname, res) == -1)
		std::cerr << "Can't save results to file " << fname << std::endl, exit(1);

	std::string fname2 = std::string(argv[1]) + ".md";

	if (main_sys.save_to_markdown(fname2, res) == -1)
		std::cerr << "Can't save results to file " << fname2 << std::endl, exit(1);
	
	return 0;
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
