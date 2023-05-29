
BUILD_DIR = build
INCLUDE_DIR = include

EXEC = es

PARSER_PATH = src/parser.y
LEXER_PATH = src/lexer.l

PARSER_OUT = $(BUILD_DIR)/parser.cpp
LEXER_OUT = $(BUILD_DIR)/lexer.cpp

SRCS = src/Expression.cpp \
		src/System.cpp

all:
	mkdir -p $(BUILD_DIR)
	bison -v -d -o $(PARSER_OUT) $(PARSER_PATH)
	flex -o $(LEXER_OUT) $(LEXER_PATH)
	clang++ --std=c++17 -I $(INCLUDE_DIR) -lgsl -lgslcblas -lm -o $(EXEC) $(PARSER_OUT) $(LEXER_OUT) $(SRCS)

run: all
	./$(EXEC) test/test.ees

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(EXEC)

re: clean all

.PHONY: all clean re
