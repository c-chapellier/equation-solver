
BUILD_DIR = build
INCLUDE_DIR = include

EXEC = es

PARSER_PATH = src/parser.y
LEXER_PATH = src/lexer.l

PARSER_OUT = $(BUILD_DIR)/parser.cpp
LEXER_OUT = $(BUILD_DIR)/lexer.cpp

SRCS = 	src/System.cpp \
		src/expressions/Exp.cpp \
		src/expressions/ExpEqu.cpp \
		src/expressions/ExpNum.cpp \
		src/expressions/ExpVar.cpp \
		src/expressions/ExpAdd.cpp \
		src/expressions/ExpSub.cpp \
		src/expressions/ExpMul.cpp \
		src/expressions/ExpDiv.cpp \
		src/expressions/ExpExp.cpp \
		src/expressions/ExpPar.cpp \
		src/expressions/ExpFuncCall.cpp \

all:
	mkdir -p $(BUILD_DIR)
	bison -v --debug -d -o $(PARSER_OUT) $(PARSER_PATH)
	flex -o $(LEXER_OUT) $(LEXER_PATH)
	clang++ --std=c++17 -I $(INCLUDE_DIR) -lgsl -lgslcblas -lm -o $(EXEC) $(PARSER_OUT) $(LEXER_OUT) $(SRCS)

run: all
	./$(EXEC) test/test.ees

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(EXEC)

re: clean all

.PHONY: all clean re
