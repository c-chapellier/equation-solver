
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

extern auto yyparse() -> int;
extern FILE* yyin;

System main_sys = System();
std::map<std::string, Function *> funcs;
int n_parsing_errors = 0;

void parse(const std::string &fname)
{
	yyin = fopen(fname.c_str(), "r");
	if (yyin == nullptr) {
		std::cerr << "Can't open file " << fname << std::endl, exit(1);
}

	do {
		yyparse();
	} while(feof(yyin) == 0);

	fclose(yyin);

	if (n_parsing_errors > 0)
	{
		std::cerr << n_parsing_errors << " error"
			<< ((n_parsing_errors != 1) ? "s" : "")
			<< " generated." << std::endl;
		exit(1);
	}

	main_sys.load_vars_from_equs();
}

auto main(int argc, char* argv[]) -> int
{
	// while (getchar() != '\n');

	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl, exit(1);
}

	std::vector<std::string> abs_args({ "x" });
	funcs["abs"] = new Function("abs", abs_args, new System(), new ExpAbs());

	std::string const fname = argv[1];
	parse(fname);

	std::vector<double> res;
	std::vector<double> guesses = std::vector<double>(main_sys.size(), 1  );
	main_sys.solve(res, guesses);

	Saver::save_to_file(fname + ".res", funcs, main_sys, res);
	Saver::save_to_markdown(fname + ".md", funcs, main_sys, res);

	delete funcs["abs"];

	// system("leaks -q es");

	return 0;
}
