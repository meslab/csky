CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wpedantic -fPIC -s
CFLAGS_RELEASE = -O2 -ffunction-sections -flto
CFLAGS_DEBUG = -g
LDFLAGS += -Wl,--gc-sections -flto

BIN_DIR = bin
MAIN_STATIC_BIN = $(BIN_DIR)/squiterrator_static
MAIN_SHARED_BIN = $(BIN_DIR)/squiterrator_shared
MAIN_SRC = $(wildcard main/*.c)

TEST_BIN_DIR = test/bin
LIB_DIR = lib
INCLUDE_DIR = include

LIB_NAME = squiterrator
STATIC_LIB = $(LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB = $(LIB_DIR)/lib$(LIB_NAME).so

CRUX_BASE_DIR = ../crux
CRUX_LIB_NAME = crux
CRUX_LIB_DIR = $(CRUX_BASE_DIR)/lib
CRUX_INCLUDE_DIR = $(CRUX_BASE_DIR)/include
CRUX_STATIC_LIB = $(CRUX_LIB_DIR)/lib$(CRUX_LIB_NAME).a
CRUX_SHARED_LIB = $(CRUX_LIB_DIR)/lib$(CRUX_LIB_NAME).so

TEST_SRC = $(wildcard test/src/*.c)
TEST_MAIN = $(wildcard test/*.c)
TEST_STATIC_BIN = $(TEST_BIN_DIR)/test_squiterrator_static
TEST_SHARED_BIN = $(TEST_BIN_DIR)/test_squiterrator_shared

# Gather all source files automatically
SRC_FILES := $(wildcard src/*.c)
OBJ_FILES := $(patsubst src/%.c, $(LIB_DIR)/%.o, $(SRC_FILES))

# Targets
all: $(STATIC_LIB) $(SHARED_LIB) test_static test_shared main_static main_shared

# Build static library
$(STATIC_LIB): $(OBJ_FILES)
	ar rcs $(STATIC_LIB) $(OBJ_FILES)

# Compile each .c file into .o inside the lib directory
$(LIB_DIR)/%.o: src/%.c
	mkdir -p $(LIB_DIR) $(TEST_BIN_DIR) $(BIN_DIR) 
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -c $< -o $@ $(LDFLAGS)

# Build shared library
$(SHARED_LIB): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -shared -o $(SHARED_LIB) $^

# Test with static library
test_static: $(STATIC_LIB) $(TEST_SRC)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(TEST_SRC) $(TEST_MAIN) $(STATIC_LIB) $(CRUX_STATIC_LIB) -o $(TEST_STATIC_BIN)

# Test with shared library
test_shared: $(SHARED_LIB) $(TEST_SRC)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(TEST_SRC) $(TEST_MAIN) -L$(LIB_DIR) -l$(LIB_NAME) -L$(CRUX_LIB_DIR) -l$(CRUX_LIB_NAME) -o $(TEST_SHARED_BIN)

# Run tests
run_test_static: test_static
	$(TEST_STATIC_BIN)

# Run tests
run_test_shared: test_shared
	LD_LIBRARY_PATH=$(LIB_DIR):$(CRUX_LIB_DIR) ./$(TEST_SHARED_BIN)

# Main with static library
main_static: $(STATIC_LIB) $(MAIN_SRC)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(MAIN_SRC) $(STATIC_LIB) $(CRUX_STATIC_LIB) -o $(MAIN_STATIC_BIN)

# Main with shared library
main_shared: $(SHARED_LIB) $(MAIN_SRC)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(MAIN_SRC) -L$(LIB_DIR) -l$(LIB_NAME) -L$(CRUX_LIB_DIR) -l$(CRUX_LIB_NAME) -o $(MAIN_SHARED_BIN)

run_main_shared: main_shared
	LD_LIBRARY_PATH=$(LIB_DIR) ./$(MAIN_SHARED_BIN)

run_main_static: main_static
	$(MAIN_STATIC_BIN) $(LOG_LEVEL)

# Clean build artifacts
clean:
	rm -rf $(LIB_DIR) $(TEST_BIN_DIR) $(BIN_DIR)

.PHONY: all test_shared test_static test clean run_shared run_static main_shared main_static
