%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

#include "../include/es.hpp"

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
#include "../src/expressions/ExpAbs.hpp"

#include "../src/Function.hpp"

#include "../src/System.hpp"

#include "../src/Saver.hpp"

System main_sys = System();

std::map<std::string, Function *> funcs;

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
	void *args_names_val;
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
%type<args_names_val> args_names
%type<func_val> func
%type<args_val> args

%start prog

%%

prog:
	| prog block 		{ }
;

block:
	  func				{ debug("block: func\n"); funcs[((Function *)$1)->name] = (Function *)$1; }
	| sys				{ debug("block: sys\n"); main_sys.add_sys((System *)$1); delete (System *)$1; }

func:
	  T_FUNC T_VAR T_LPAR args_names T_RPAR T_NEWLINE T_LBRA T_NEWLINE sys T_RETURN exp T_NEWLINE T_RBRA { debug("func:\n"); $$ = new Function(std::string($2), (std::vector<std::string> *)$4, (System *)$9, (Exp *)$11); delete $2; }
	| T_FUNC T_VAR T_LPAR args_names T_RPAR T_NEWLINE T_LBRA T_NEWLINE T_RETURN exp T_NEWLINE T_RBRA { debug("func:\n"); $$ = new Function(std::string($2), (std::vector<std::string> *)$4, new System(), (Exp *)$10); delete $2; }
;

args_names:
	  T_VAR T_COMMA args_names	{ debug("args_names: T_VAR T_COMMA args_names\n"); $$ = $3; ((std::vector<std::string> *)$$)->insert(((std::vector<std::string> *)$$)->begin(), std::string($1)); delete $1; }
	| T_VAR						{ debug("args_names: T_VAR\n"); $$ = new std::vector<std::string>(); ((std::vector<std::string> *)$$)->insert(((std::vector<std::string> *)$$)->begin(), std::string($1)); delete $1; }
;

sys:
	  T_NEWLINE				{ debug("sys: T_NEWLINE\n"); $$ = new System(); }
	| equ T_NEWLINE			{ debug("sys: equ T_NEWLINE\n"); $$ = new System(); ((System *)$$)->add_equ((Exp *)$1); }
	| equ T_EOF				{ debug("sys: equ T_EOF\n"); $$ = new System(); ((System *)$$)->add_equ((Exp *)$1); }
	| sys equ T_NEWLINE		{ debug("sys: sys equ T_NEWLINE\n"); $$ = $1; ((System *)$$)->add_equ((Exp *)$2); }
	| sys T_NEWLINE			{ debug("sys: sys T_NEWLINE\n"); }
	| sys T_EOF				{ debug("sys: sys T_EOF\n"); }
;

equ: exp T_EQU exp		{ debug("equ: exp T_EQU exp\n"); $$ = new ExpEqu((Exp *)$1, (Exp *)$3); }
;

exp:
	  T_DOUBLE			{ debug("exp: T_DOUBLE(%f)\n", $1); $$ = new ExpNum($1); }
	| T_VAR				{ debug("exp: T_VAR(%s)\n", $1); $$ = new ExpVar($1); delete $1; }
	| exp T_ADD exp		{ debug("exp: exp T_ADD exp\n"); $$ = new ExpAdd((Exp *)$1, (Exp *)$3); }
	| exp T_SUB exp		{ debug("exp: exp T_SUB exp\n"); $$ = new ExpSub((Exp *)$1, (Exp *)$3); }
	| exp T_MUL exp		{ debug("exp: exp T_MUL exp\n"); $$ = new ExpMul((Exp *)$1, (Exp *)$3); }
	| exp T_DIV exp		{ debug("exp: exp T_DIV exp\n"); $$ = new ExpDiv((Exp *)$1, (Exp *)$3); }
	| exp T_EXP exp		{ debug("exp: exp T_EXP exp\n"); $$ = new ExpExp((Exp *)$1, (Exp *)$3); }
	| T_LPAR exp T_RPAR	{ debug("exp: T_LPAR exp T_RPAR\n"); $$ = new ExpPar((Exp *)$2); }
	| T_VAR T_LPAR args T_RPAR	{ debug("exp: T_VAR T_LPAR args T_RPAR\n"); $$ = new ExpFuncCall(funcs[$1]->deep_copy(), (std::vector<Exp *> *)$3); delete $1; }
;

args:
	  exp T_COMMA args	{ debug("args: exp T_COMMA args\n"); $$ = $3; ((std::vector<Exp *> *)$$)->push_back((Exp *)$1); }
	| exp				{ debug("args: exp\n"); $$ = new std::vector<Exp *>(); ((std::vector<Exp *> *)$$)->push_back((Exp *)$1); }

%%

int main(int argc, char* argv[])
{
	if (argc != 2)
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl, exit(1);

	std::string fname(argv[1]);
	
	yyin = fopen(argv[1], "r");
	if (yyin == NULL)
		std::cerr << "Can't open file " << argv[1] << std::endl, exit(1);

	funcs["abs"] = new Function("abs", new std::vector<std::string>({ "x" }), new System(), new ExpAbs());

	do
		yyparse();
	while(!feof(yyin));

	fclose(yyin);

	main_sys.load_vars_from_equs();

	printf("----------- SYSTEM PARSED ------------\n");
	main_sys.print();
	printf("----------- SOLVE ------------\n");

	std::vector<double> res;
	std::vector<double> guesses = std::vector<double>(main_sys.size(), 1  );
	main_sys.solve(res, guesses);

	debug("Solution:\n");
	for (int i = 0; i < main_sys.size(); ++i)
		debug("  %s = %f\n", main_sys.vars[i].c_str(), res[i]);

	Saver::save_to_file(fname + ".res", funcs, main_sys, res);
	Saver::save_to_markdown(fname + ".md", funcs, main_sys, res);

	delete funcs["abs"];

	system("leaks es");

	return 0;
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
