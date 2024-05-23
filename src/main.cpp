
#include "es.hpp"

extern int yyparse();
extern FILE* yyin;

System main_sys = System();
int n_parsing_errors = 0;

static void parse_file(const std::string &fname)
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
}

// static void add_default_funcs()
// {
// 	for (auto &func_data : default_funcs)
// 	{
// 		funcs[func_data.name] = new Function(
// 			func_data.name,
// 			func_data.args,
// 			new System(),
// 			new ExpCustom(func_data.args.size(), func_data.func, func_data.str_repr, func_data.latex_repr)
// 		);
// 	}
// }

int main(int argc, char* argv[])
{
	if (argc != 2)
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl, exit(1);
	
	// add_default_funcs();

	parse_file(argv[1]);

	main_sys.add_equs_from_func_calls();
	main_sys.singularize_vars();
	main_sys.infer();
	main_sys.solve();

	// Saver::save_to_file(std::string(argv[1]) + ".res", funcs, main_sys);
	Saver::save_to_markdown(std::string(argv[1]) + ".md", funcs, main_sys);

	for (auto &func : funcs)
		delete func.second;

	return 0;
}
