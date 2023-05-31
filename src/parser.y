%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

#include "es.hpp"

#include "../src/expressions/Exp.hpp"
#include "../src/expressions/ExpEqu.hpp"
#include "../src/expressions/ExpNum.hpp"
#include "../src/expressions/ExpVar.hpp"
#include "../src/expressions/ExpAdd.hpp"
#include "../src/expressions/ExpSub.hpp"
#include "../src/expressions/ExpMul.hpp"
#include "../src/expressions/ExpDiv.hpp"
#include "../src/expressions/ExpExp.hpp"
#include "../src/expressions/ExpPar.hpp"
#include "../src/expressions/ExpFuncCall.hpp"

#include "../src/Function.hpp"

#include "../src/System.hpp"

System main_sys = System();

std::vector<Function *> functions;

Exp *create_function_call(std::string name, std::vector<Exp *> *args)
{
	Function *f = NULL;
	for (int i = 0; i < functions.size(); ++i)
		if (functions[i]->name == name)
			f = functions[i];
	
	if (f == NULL)
		yyerror("Function not found");

	if (f->args_names->size() != args->size())
		yyerror("Wrong number of arguments");

	ExpFuncCall *exp = new ExpFuncCall(f->name, new System());

	for (int i = 0; i < args->size(); ++i)
		exp->args.push_back((*args)[i]);

	for (int i = 0; i < f->args_names->size(); ++i)
	{
		if (dynamic_cast<ExpEqu *>((*args)[i]) != nullptr)
			yyerror("Can't use equation as argument");

		exp->sys->add_equ(
			new ExpEqu(
				new ExpVar(std::string("@") + (*f->args_names)[i]),
				(*args)[i]
			)
		);
	}

	for (int i = 0; i < f->sys->size(); ++i)
		exp->sys->add_equ(f->sys->equs[i]);

	exp->sys->add_equ(
		new ExpEqu(
			new ExpVar(std::string("#ret")),
			f->exp
		)
	);

	exp->sys->load_vars_from_equs();

	return exp;
}

%}

%define "api.pure" "full"
%define "api.token.prefix" "T_"
%define "api.value.type" "union"
%define "parse.error" "detailed"
%define "parse.trace"

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
	| prog block 		{ }
;

block:
	  func				{ functions.push_back((Function *)$1); }
	| sys				{ main_sys.add_sys((System *)$1); }

func:
	  T_FUNC T_VAR T_LPAR args_def T_RPAR T_NEWLINE T_LBRA T_NEWLINE sys T_RETURN exp T_NEWLINE T_RBRA
	  { $$ = new Function(std::string($2), (std::vector<std::string> *)$4, (System *)$9, (Exp *)$11); }
;

args_def:
	  T_VAR T_COMMA args_def	{ ((std::vector<std::string> *)$$)->push_back(std::string($1)); }
	| T_VAR						{ $$ = new std::vector<std::string>(); ((std::vector<std::string> *)$$)->push_back(std::string($1)); }
;

sys:
	  T_NEWLINE				{ $$ = new System(); }
	| equ T_NEWLINE			{ $$ = new System(); ((System *)$$)->add_equ((Exp *)$1); }
	| equ T_EOF				{ $$ = new System(); ((System *)$$)->add_equ((Exp *)$1); }
	| sys equ T_NEWLINE		{ ((System *)$$)->add_equ((Exp *)$2); }
	| sys T_NEWLINE			{ }
	| sys T_EOF				{ }
;

equ: exp T_EQU exp		{ $$ = new ExpEqu((Exp *)$1, (Exp *)$3); }
;

exp:
	  T_DOUBLE			{ $$ = new ExpNum($1); }
	| T_VAR				{ $$ = new ExpVar($1); }
	| exp T_ADD exp		{ $$ = new ExpAdd((Exp *)$1, (Exp *)$3) }
	| exp T_SUB exp		{ $$ = new ExpSub((Exp *)$1, (Exp *)$3) }
	| exp T_MUL exp		{ $$ = new ExpMul((Exp *)$1, (Exp *)$3) }
	| exp T_DIV exp		{ $$ = new ExpDiv((Exp *)$1, (Exp *)$3) }
	| exp T_EXP exp		{ $$ = new ExpExp((Exp *)$1, (Exp *)$3) }
	| T_LPAR exp T_RPAR	{ $$ = new ExpPar((Exp *)$2) }
	| T_VAR T_LPAR args T_RPAR	{ $$ = create_function_call(std::string($1), (std::vector<Exp *> *)$3); }
;

args:
	  exp T_COMMA args	{ ((std::vector<Exp *> *)$$)->push_back((Exp *)$1); }
	| exp				{ $$ = new std::vector<Exp *>(); ((std::vector<Exp *> *)$$)->push_back((Exp *)$1); }

%%

int main(int argc, char* argv[])
{
	if (argc != 2)
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl, exit(1);

	// yydebug = 1;

	std::string fname(argv[1]);
	
	yyin = fopen(argv[1], "r");
	if (yyin == NULL)
		std::cerr << "Can't open file " << argv[1] << std::endl, exit(1);

	do
		yyparse();
	while(!feof(yyin));

	fclose(yyin);

	main_sys.load_vars_from_equs();

	main_sys.print();
	printf("----------- SOLVE ------------\n");

	std::vector<double> res;
	main_sys.solve(res);

	debug("Solution:\n");
	for (int i = 0; i < main_sys.size(); ++i)
		debug("  %s = %f\n", main_sys.vars[i].c_str(), res[i]);

	if (main_sys.save_to_file(fname + ".res", res) == -1)
		std::cerr << "Can't save results to file " << fname  + ".res" << std::endl, exit(1);

	if (main_sys.save_to_markdown(fname + ".md", res) == -1)
		std::cerr << "Can't save results to file " << fname  + ".md" << std::endl, exit(1);
	
	return 0;
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
