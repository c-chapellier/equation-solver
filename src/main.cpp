
#include "es.hpp"

extern int yyparse();
extern FILE* yyin;

System main_sys = System();
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

	if (argc != 2)
		std::cerr << "Usage: " << args[0] << " <filename>" << std::endl, exit(1);
	
	for (auto &func_data : default_funcs)
	{
		funcs[func_data.name] = new Function(
			func_data.name,
			func_data.args,
			new System(),
			new ExpCustom(func_data.args.size(), func_data.func, func_data.str_repr, func_data.latex_repr)
		);
	}

	std::string fname(args[1]);
	parse(fname);

	// for (auto &var : default_vars)
	// {
	// 	main_sys.add_equ(new ExpEqu(new ExpVar(var.first), new ExpNum(var.second)));
	// 	main_sys.add_var(var.first);
	// }

	std::vector<double> res;
	std::vector<double> guesses = std::vector<double>(main_sys.size(), 1  );
	main_sys.solve(res, guesses);

	Saver::save_to_file(fname + ".res", funcs, main_sys, res);
	Saver::save_to_markdown(fname + ".md", funcs, main_sys, res);

	for (auto &func : funcs)
		delete func.second;

	return 0;
}
