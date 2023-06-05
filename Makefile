
# path #
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

# executable # 
BIN_NAME = es

# extensions #
SRC_EXT = cpp
HEADER_EXT = hpp

PARSER_PATH = src/parser.y
LEXER_PATH = src/lexer.l

PARSER_OUT = $(SRC_PATH)/parser.cpp
LEXER_OUT = $(SRC_PATH)/lexer.cpp

# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

# flags #
COMPILE_FLAGS = --std=c++17 -g
INCLUDES = -I include/ -I /usr/local/include
# Space-separated pkg-config libraries used by this project
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

# checks the executable and symlinks to the output
.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	clang++ $(OBJECTS) -o $@ ${LIBS} -lSDL2

# Flex and Bison
.PHONY: bison
bison:
	@echo "Bison: $(PARSER_PATH) -> $(PARSER_OUT)"
	bison --debug -d -o $(PARSER_OUT) $(PARSER_PATH)

.PHONY: flex
flex:
	@echo "Flex: $(LEXER_PATH) -> $(LEXER_OUT)"
	flex -o $(LEXER_OUT) $(LEXER_PATH)

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	clang++ $(CFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
