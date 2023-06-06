
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

BIN_NAME = es

SRC_EXT = cpp
HEADER_EXT = hpp

PARSER_PATH = src/parser.y
LEXER_PATH = src/lexer.l

PARSER_OUT = $(SRC_PATH)/parser.cpp
LEXER_OUT = $(SRC_PATH)/lexer.cpp

SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
DEPS = $(OBJECTS:.o=.d)

COMPILE_FLAGS = --std=c++17 -g -O0
INCLUDES = -Iinclude/ -I/usr/local/include
LIBS = -lgsl -lgslcblas -lm

.PHONY: default_target
default_target: release

.PHONY: release
release: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)
	@echo "Deleting parser and lexer"
	@$(RM) -r $(PARSER_OUT) $(PARSER_OUT:%.$(SRC_EXT)=%.$(HEADER_EXT)) $(LEXER_OUT)

.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	clang++ $(OBJECTS) -o $@ ${LIBS} -lSDL2

.PHONY: parser
parser:
	@echo "Creating parser: $(PARSER_PATH) -> $(PARSER_OUT)"
	bison --debug -d -o $(PARSER_OUT) $(PARSER_PATH)

.PHONY: lexer
lexer:
	@echo "Creating lexer: $(LEXER_PATH) -> $(LEXER_OUT)"
	flex -o $(LEXER_OUT) $(LEXER_PATH)

.PHONY: static-analyzer
static-analyzer:
	@echo "Running static analyzer"
	clang-tidy $(SOURCES) -- $(CFLAGS) $(INCLUDES)

-include $(DEPS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	clang++ $(CFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
