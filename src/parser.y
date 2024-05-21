%{

extern int yylex();

typedef struct yyltype
{
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} yyltype;

void yyerror2(void *loc, const char *s);

#include "../include/es.hpp"

template<typename T>
constexpr void debug(const T &x)
{
    if (0) std::cerr << x << std::endl;
}

#define YYERROR_VERBOSE 1
#define yyerror(msg) yyerror2(&yylloc, msg)

extern System main_sys;
extern std::map<std::string, Function *> funcs;
extern int n_parsing_errors;

%}

%locations

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
%token<sval> T_UNIT
%token<sval> T_GUESS

%token T_EQU T_ADD T_SUB T_MUL T_DIV T_POW
%token T_LPAR T_RPAR T_LBRA T_RBRA T_COMMA
%token T_RETURN T_FUNC
%token T_NEWLINE T_EOF

%left T_ADD T_SUB
%left T_MUL T_DIV
%left T_POW

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
	  T_FUNC T_VAR T_LPAR args_names T_RPAR T_NEWLINE T_LBRA T_NEWLINE sys T_RETURN exp T_NEWLINE T_RBRA { debug("func:\n"); $$ = new Function(std::string($2), *((std::vector<std::string> *)$4), (System *)$9, (Exp *)$11); delete $2; delete (std::vector<std::string> *)$4; }
	| T_FUNC T_VAR T_LPAR args_names T_RPAR T_NEWLINE T_LBRA T_NEWLINE T_RETURN exp T_NEWLINE T_RBRA { debug("func:\n"); $$ = new Function(std::string($2), *((std::vector<std::string> *)$4), new System(), (Exp *)$10); delete $2; delete (std::vector<std::string> *)$4; }
;

args_names:
	  T_VAR T_COMMA args_names	{ debug("args_names: T_VAR T_COMMA args_names"); $$ = $3; ((std::vector<std::string> *)$$)->insert(((std::vector<std::string> *)$$)->begin(), std::string($1)); delete $1; }
	| T_VAR						{ debug("args_names: T_VAR"); $$ = new std::vector<std::string>(); ((std::vector<std::string> *)$$)->insert(((std::vector<std::string> *)$$)->begin(), std::string($1)); delete $1; }
;

sys:
	  T_NEWLINE				{ debug("sys: T_NEWLINE"); $$ = new System(); }
	| equ T_NEWLINE			{ debug("sys: equ T_NEWLINE"); $$ = new System(); ((System *)$$)->add_equ((ExpOp *)$1); }
	| equ T_EOF				{ debug("sys: equ T_EOF"); $$ = new System(); ((System *)$$)->add_equ((ExpOp *)$1); }
	| sys equ T_NEWLINE		{ debug("sys: sys equ T_NEWLINE"); $$ = $1; ((System *)$$)->add_equ((ExpOp *)$2); }
	| sys T_NEWLINE			{ debug("sys: sys T_NEWLINE"); }
	| sys T_EOF				{ debug("sys: sys T_EOF"); }
;

equ: exp T_EQU exp		{ debug("equ: exp T_EQU exp"); $$ = new ExpOp(OpType::EQU, (Exp *)$1, (Exp *)$3); }
;

exp:
	  T_DOUBLE			{ debug("exp: T_DOUBLE(" + std::to_string($1) + ")"); $$ = new ExpNum($1); }
	| T_DOUBLE T_UNIT	{ debug("exp: T_UNIT(" + std::to_string($1) + ", " + std::string($2) + ")"); $$ = new ExpNum($1, $2); delete $2; }
	| T_VAR				{ debug("exp: T_VAR(" + std::string($1) + ")"); $$ = new ExpVar($1); delete $1; }
	| T_VAR T_GUESS		{ debug("exp: T_GUESS(" + std::string($1) + ", " + std::string($2) + ")"); $$ = new ExpVar($1, $2); delete $1; delete $2; }
	| exp T_ADD exp		{ debug("exp: exp T_ADD exp"); $$ = new ExpOp(OpType::ADD, (Exp *)$1, (Exp *)$3); }
	| exp T_SUB exp		{ debug("exp: exp T_SUB exp"); $$ = new ExpOp(OpType::SUB, (Exp *)$1, (Exp *)$3); }
	| exp T_MUL exp		{ debug("exp: exp T_MUL exp"); $$ = new ExpOp(OpType::MUL, (Exp *)$1, (Exp *)$3); }
	| exp T_DIV exp		{ debug("exp: exp T_DIV exp"); $$ = new ExpOp(OpType::DIV, (Exp *)$1, (Exp *)$3); }
	| exp T_POW exp		{ debug("exp: exp T_POW exp"); $$ = new ExpOp(OpType::POW, (Exp *)$1, (Exp *)$3); }
	| T_LPAR exp T_RPAR	{ debug("exp: T_LPAR exp T_RPAR"); $$ = new ExpOp(OpType::PAR, (Exp *)$2, NULL); }
	| T_VAR T_LPAR args T_RPAR	{ debug("exp: T_VAR T_LPAR args T_RPAR"); $$ = new ExpFuncCall(funcs[$1], *(std::vector<Exp *> *)$3); delete $1; delete (std::vector<Exp *> *)$3; }
;

args:
	  exp T_COMMA args	{ debug("args: exp T_COMMA args"); $$ = $3; ((std::vector<Exp *> *)$$)->push_back((Exp *)$1); }
	| exp				{ debug("args: exp"); $$ = new std::vector<Exp *>(); ((std::vector<Exp *> *)$$)->push_back((Exp *)$1); }

%%

void yyerror2(void *loc, const char *s)
{
	yyltype *yylloc = (yyltype *)loc;
	std::cerr << "\033[1mtest/test.es:" << yylloc->first_line << ":" << yylloc->first_column << ": \033[1;31merror:\033[0m\033[1m " << s << "\033[0m" << std::endl;
	++n_parsing_errors;
}
