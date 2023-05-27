
BUILD_DIR = build

PARSER_PATH = src/parser.y
LEXER_PATH = src/lexer.l

PARSER_OUT = $(BUILD_DIR)/parser.c
LEXER_OUT = $(BUILD_DIR)/lexer.c

SRCS = src/nonlinear_equ_sys_solver.c src/equ.c src/expr.c src/sys.c

EXEC = es

all:
	mkdir -p $(BUILD_DIR)
	bison -v -d -o $(PARSER_OUT) $(PARSER_PATH)
	flex -o $(LEXER_OUT) $(LEXER_PATH)
	clang -I include -lgsl -lgslcblas -lm -o $(EXEC) $(BUILD_DIR)/parser.c $(BUILD_DIR)/lexer.c $(SRCS)

run: all
	./$(EXEC) test/temp.ees

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(EXEC)

re: clean all

.PHONY: all clean re
