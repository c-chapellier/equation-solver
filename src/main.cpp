
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

typedef struct default_func_s
{
	std::string name;
	std::vector<std::string> args;
	double (*func)(double *);
	const char *str_repr;
	const char *latex_repr;
} default_func_t;

static default_func_t default_funcs[] = {
	{ "abs", { "x" }, [](double *args) -> double { return abs(args[0]); }, "|x|", "\\mid x \\mid" },
	{ "sin", { "x" }, [](double *args) -> double { return sin(args[0]); }, "sin(x)", "\\sin(x)" },
	{ "cos", { "x" }, [](double *args) -> double { return cos(args[0]); }, "cos(x)", "\\cos(x)" },
	{ "tan", { "x" }, [](double *args) -> double { return tan(args[0]); }, "tan(x)", "\\tan(x)" },
	{ "exp", { "x" }, [](double *args) -> double { return exp(args[0]); }, "exp(x)", "e^{x}" },
	{ "log", { "x" }, [](double *args) -> double { return log(args[0]); }, "log(x)", "\\log(x)" },
	{ "sqrt", { "x" }, [](double *args) -> double { return sqrt(args[0]); }, "sqrt(x)", "\\sqrt{x}" },
	{ "atan2", { "x", "y" }, [](double *args) -> double { return atan2(args[0], args[1]); }, "atan2(x, y)", "\\text{atan2}(x, y)" },
	{ "hypot", { "x", "y" }, [](double *args) -> double { return hypot(args[0], args[1]); }, "hypot(x, y)", "\\text{hypot}(x, y)" },
	{ "min", { "a", "b" }, [](double *args) -> double { return std::min(args[0], args[1]); }, "min(x, y)", "\\min(x, y)" },
	{ "max", { "a", "b" }, [](double *args) -> double { return std::max(args[0], args[1]); }, "max(x, y)", "\\max(x, y)" }
};

static void add_default_funcs()
{
	for (auto &func_data : default_funcs)
	{
		funcs[func_data.name] = new Function(
			func_data.name,
			func_data.args,
			new System(),
			new ExpCustom(func_data.args.size(), func_data.func, func_data.str_repr, func_data.latex_repr)
		);
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl, exit(1);
	
	add_default_funcs();

	parse_file(argv[1]);

	main_sys.add_equs_from_func_calls();
	main_sys.singularize_vars();
	main_sys.infer();

	// std::cout << main_sys;
	main_sys.solve();

	// Saver::save_to_file(std::string(argv[1]) + ".res", main_sys);
	Saver::save_to_markdown(std::string(argv[1]) + ".md", main_sys);

	for (auto &func : funcs)
		delete func.second;

	std::cout << "Done." << std::endl;

	return 0;
}
