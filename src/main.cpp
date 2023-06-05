
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

extern int yyparse();
extern FILE* yyin;

System main_sys = System();
std::map<std::string, Function *> funcs;

void parse(const std::string &fname)
{
	yyin = fopen(fname.c_str(), "r");
	if (yyin == NULL)
		std::cerr << "Can't open file " << fname << std::endl, exit(1);

	do
		yyparse();
	while(!feof(yyin));

	fclose(yyin);

	main_sys.load_vars_from_equs();
}

int main(int argc, char* argv[])
{
	// while (getchar() != '\n');

	if (argc != 2)
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl, exit(1);

	std::vector<std::string> abs_args({ "x" });
	funcs["abs"] = new Function("abs", abs_args, new System(), new ExpAbs());

	std::string fname = argv[1];
	
	parse(fname);

	std::vector<double> res;
	std::vector<double> guesses = std::vector<double>(main_sys.size(), 1  );
	main_sys.solve(res, guesses);

	debug("Solution:\n");
	for (int i = 0; i < main_sys.size(); ++i)
		debug("  %s = %f\n", main_sys.vars[i].c_str(), res[i]);

	Saver::save_to_file(fname + ".res", funcs, main_sys, res);
	Saver::save_to_markdown(fname + ".md", funcs, main_sys, res);

	delete funcs["abs"];

	system("leaks -q es");

	return 0;
}
