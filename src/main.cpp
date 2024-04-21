
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

	if (argc != 2)
		std::cerr << "Usage: " << args[0] << " <filename>" << std::endl, exit(1);


	struct FuncData
	{
		std::string name;
		std::vector<std::string> args;
		double (*func)(double *);
		const char *str_repr;
		const char *latex_repr;
	};

	FuncData funcs_data[] = {
		{ "abs", { "x" }, [](double *args) -> double { return abs(args[0]); }, "\\mid x \\mid", "|x|" },
		{ "sin", { "x" }, [](double *args) -> double { return sin(args[0]); }, "\\sin(x)", "\\sin(x)" },
		{ "cos", { "x" }, [](double *args) -> double { return cos(args[0]); }, "\\cos(x)", "\\cos(x)" },
		{ "tan", { "x" }, [](double *args) -> double { return tan(args[0]); }, "\\tan(x)", "\\tan(x)" },
		{ "exp", { "x" }, [](double *args) -> double { return exp(args[0]); }, "\\exp(x)", "e^{x}" },
		{ "log", { "x" }, [](double *args) -> double { return log(args[0]); }, "\\log(x)", "\\log(x)" },
		{ "sqrt", { "x" }, [](double *args) -> double { return sqrt(args[0]); }, "\\sqrt{x}", "\\sqrt{x}" },
		{ "atan2", { "x", "y" }, [](double *args) -> double { return atan2(args[0], args[1]); }, "\\text{atan2}(x, y)", "\\text{atan2}(x, y)" },
		{ "hypot", { "x", "y" }, [](double *args) -> double { return hypot(args[0], args[1]); }, "\\text{hypot}(x, y)", "\\text{hypot}(x, y)" },
		{ "min", { "a", "b" }, [](double *args) -> double { return std::min(args[0], args[1]); }, "min(x, y)", "\\min(x, y)" },
		{ "max", { "a", "b" }, [](double *args) -> double { return std::max(args[0], args[1]); }, "max(x, y)", "\\max(x, y)" }
	};
	
	for (auto &func_data : funcs_data)
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

	std::map<std::string, double> vars = {
		{ "pi", M_PI },
		{ "e", M_E },
	};

	for (auto &var : vars)
	{
		std::cout << "Adding var: " << var.first << " = " << var.second << std::endl;
		main_sys.add_equ(new ExpEqu(new ExpVar(var.first), new ExpNum(var.second)));
		main_sys.add_var(var.first);
	}


	std::vector<double> res;
	std::vector<double> guesses = std::vector<double>(main_sys.size(), 1  );
	main_sys.solve(res, guesses);

	Saver::save_to_file(fname + ".res", funcs, main_sys, res);
	Saver::save_to_markdown(fname + ".md", funcs, main_sys, res);

	delete funcs["abs"];

	// system("leaks -q es");

	return 0;
}
