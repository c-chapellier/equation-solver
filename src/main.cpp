
#include "es.hpp"

extern int yyparse();
extern FILE* yyin;

System main_sys = System();
std::map<std::string, Function *> funcs;
int n_parsing_errors = 0;

void parse(const std::string &fname)
{
	yyin = fopen(fname.c_str(), "r");
	if (yyin == NULL)
		std::cerr << "Can't open file " << fname << std::endl, exit(1);

	do
		yyparse();
	while(!feof(yyin));

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

int main(int argc, char* argv[])
{
	std::span<char *> args = std::span(argv, size_t (argc));

	std::string exec_name(args[0]);

	if (argc != 2)
		std::cerr << "Usage: " << exec_name << " <filename>" << std::endl, exit(1);

	std::vector<std::string> abs_args({ "x" });
	funcs["abs"] = new Function("abs", abs_args, new System(), new ExpAbs());

	std::string fname(args[1]);
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
