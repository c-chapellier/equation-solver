
BUILD_DIR = build
INCLUDE_DIR = include

EXEC = es

PARSER_PATH = src/parser.y
LEXER_PATH = src/lexer.l

PARSER_OUT = $(BUILD_DIR)/parser.c
LEXER_OUT = $(BUILD_DIR)/lexer.c

SRCS = src/nonlinear_equ_sys_solver.c \
		src/expr.c \
		src/sys.c

all:
	mkdir -p $(BUILD_DIR)
	bison -v -d -o $(PARSER_OUT) $(PARSER_PATH)
	flex -o $(LEXER_OUT) $(LEXER_PATH)
	clang -I $(INCLUDE_DIR) -lgsl -lgslcblas -lm -o $(EXEC) $(PARSER_OUT) $(LEXER_OUT) $(SRCS)

run: all
	./$(EXEC) test/test.ees

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(EXEC)

re: clean all

.PHONY: all clean re
