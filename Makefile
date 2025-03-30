CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -fPIC
CFLAGS_RELEASE = -O2 -ffunction-sections -flto -DNDEBUG
CFLAGS_DEBUG = -g -O0 -DDEBUG -Werror -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS_RELEASE = -Wl,--gc-sections -flto -s
LDFLAGS_DEBUG = -fsanitize=address,undefined

BIN_DIR = bin
DEBUG_BIN_DIR = $(BIN_DIR)/debug
RELEASE_BIN_DIR = $(BIN_DIR)/release
MAIN_STATIC_BIN_RELEASE = $(RELEASE_BIN_DIR)/csky_static
MAIN_SHARED_BIN_RELEASE = $(RELEASE_BIN_DIR)/csky_shared
MAIN_STATIC_BIN_DEBUG = $(DEBUG_BIN_DIR)/csky_static
MAIN_SHARED_BIN_DEBUG = $(DEBUG_BIN_DIR)/csky_shared
MAIN_SRC = $(wildcard main/*.c)

TEST_BIN_DIR = test/bin
TEST_DEBUG_BIN_DIR = $(TEST_BIN_DIR)/debug
TEST_RELEASE_BIN_DIR = $(TEST_BIN_DIR)/release
LIB_DIR = lib
DEBUG_LIB_DIR = $(LIB_DIR)/debug
RELEASE_LIB_DIR = $(LIB_DIR)/release
INCLUDE_DIR = include
CRUX_INCLUDE_DIR = ../crux/include

LIB_NAME = csky
STATIC_LIB_RELEASE = $(RELEASE_LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB_RELEASE = $(RELEASE_LIB_DIR)/lib$(LIB_NAME).so
STATIC_LIB_DEBUG = $(DEBUG_LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB_DEBUG = $(DEBUG_LIB_DIR)/lib$(LIB_NAME).so

CRUX_BASE_DIR = ../crux
CRUX_LIB_NAME = crux
CRUX_LIB_DIR = $(CRUX_BASE_DIR)/lib
CRUX_DEBUG_LIB_DIR = $(CRUX_LIB_DIR)/debug
CRUX_RELEASE_LIB_DIR = $(CRUX_LIB_DIR)/release
CRUX_INCLUDE_DIR = $(CRUX_BASE_DIR)/include
CRUX_STATIC_LIB_RELEASE = $(CRUX_RELEASE_LIB_DIR)/lib$(CRUX_LIB_NAME).a
CRUX_SHARED_LIB_RELEASE = $(CRUX_RELEASE_LIB_DIR)/lib$(CRUX_LIB_NAME).so
CRUX_STATIC_LIB_DEBUG = $(CRUX_DEBUG_LIB_DIR)/lib$(CRUX_LIB_NAME).a
CRUX_SHARED_LIB_DEBUG = $(CRUX_DEBUG_LIB_DIR)/lib$(CRUX_LIB_NAME).so

TEST_SRC = $(wildcard test/src/*.c)
TEST_MAIN = $(wildcard test/*.c)
TEST_STATIC_BIN_RELEASE = $(TEST_RELEASE_BIN_DIR)/test_csky_static
TEST_SHARED_BIN_RELEASE = $(TEST_RELEASE_BIN_DIR)/test_csky_shared
TEST_STATIC_BIN_DEBUG = $(TEST_DEBUG_BIN_DIR)/test_csky_static
TEST_SHARED_BIN_DEBUG = $(TEST_DEBUG_BIN_DIR)/test_csky_shared

# Gather all source files automatically
SRC_FILES := $(wildcard src/*.c)
RELEASE_OBJ_FILES := $(patsubst src/%.c, $(RELEASE_LIB_DIR)/%.o, $(SRC_FILES))
DEBUG_OBJ_FILES := $(patsubst src/%.c, $(DEBUG_LIB_DIR)/%.o, $(SRC_FILES))

# Targets
all: debug release

release: $(STATIC_LIB_RELEASE) $(SHARED_LIB_RELEASE) test_release main_release

debug: $(STATIC_LIB_DEBUG) $(SHARED_LIB_DEBUG) test_debug main_debug

# Build release static library
$(STATIC_LIB_RELEASE): $(RELEASE_OBJ_FILES)
	ar rcs $@ $^

# Build debug static library
$(STATIC_LIB_DEBUG): $(DEBUG_OBJ_FILES)
	ar rcs $@ $^

# Compile each .c file into .o for release
$(RELEASE_LIB_DIR)/%.o: src/%.c
	mkdir -p $(@D) $(TEST_RELEASE_BIN_DIR) $(RELEASE_BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) -c $< -o $@

# Compile each .c file into .o for debug
$(DEBUG_LIB_DIR)/%.o: src/%.c
	mkdir -p $(@D) $(TEST_DEBUG_BIN_DIR) $(DEBUG_BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) -c $< -o $@

# Build release shared library
$(SHARED_LIB_RELEASE): $(RELEASE_OBJ_FILES)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) $(LDFLAGS_RELEASE) -I$(INCLUDE_DIR) -shared -o $@ $^

# Build debug shared library
$(SHARED_LIB_DEBUG): $(DEBUG_OBJ_FILES)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $(LDFLAGS_DEBUG) -I$(INCLUDE_DIR) -shared -o $@ $^

# Test with release configuration
test_release: $(STATIC_LIB_RELEASE) $(SHARED_LIB_RELEASE)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(TEST_MAIN) $(STATIC_LIB_RELEASE) $(CRUX_STATIC_LIB_RELEASE) -o $(TEST_STATIC_BIN_RELEASE)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(TEST_MAIN) -L$(RELEASE_LIB_DIR) -l$(LIB_NAME) -L$(CRUX_RELEASE_LIB_DIR) -l$(CRUX_LIB_NAME) -o $(TEST_SHARED_BIN_RELEASE)

# Test with debug configuration
test_debug: $(STATIC_LIB_DEBUG) $(SHARED_LIB_DEBUG)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(TEST_MAIN) $(STATIC_LIB_DEBUG) $(CRUX_STATIC_LIB_DEBUG) -o $(TEST_STATIC_BIN_DEBUG)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(TEST_MAIN) -L$(DEBUG_LIB_DIR) -l$(LIB_NAME) -L$(CRUX_DEBUG_LIB_DIR) -l$(CRUX_LIB_NAME) -o $(TEST_SHARED_BIN_DEBUG)

# Main with release configuration
main_release: $(STATIC_LIB_RELEASE) $(SHARED_LIB_RELEASE) $(MAIN_SRC)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(MAIN_SRC) $(STATIC_LIB_RELEASE) $(CRUX_STATIC_LIB_RELEASE) -o $(MAIN_STATIC_BIN_RELEASE)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(MAIN_SRC) -L$(RELEASE_LIB_DIR) -l$(LIB_NAME) -L$(CRUX_RELEASE_LIB_DIR) -l$(CRUX_LIB_NAME) -o $(MAIN_SHARED_BIN_RELEASE)

# Main with debug configuration
main_debug: $(STATIC_LIB_DEBUG) $(SHARED_LIB_DEBUG) $(MAIN_SRC)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(MAIN_SRC) $(STATIC_LIB_DEBUG) $(CRUX_STATIC_LIB_DEBUG) -o $(MAIN_STATIC_BIN_DEBUG)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -I$(INCLUDE_DIR) -I$(CRUX_INCLUDE_DIR) $(MAIN_SRC) -L$(DEBUG_LIB_DIR) -l$(LIB_NAME) -L$(CRUX_DEBUG_LIB_DIR) -l$(CRUX_LIB_NAME) -o $(MAIN_SHARED_BIN_DEBUG)

# Run tests
run_test_release_static: test_release
	$(TEST_STATIC_BIN_RELEASE)

run_test_release_shared: test_release
	LD_LIBRARY_PATH=$(RELEASE_LIB_DIR):$(CRUX_RELEASE_LIB_DIR) ./$(TEST_SHARED_BIN_RELEASE)

run_test_debug_static: test_debug
	$(TEST_STATIC_BIN_DEBUG)

run_test_debug_shared: test_debug
	LD_LIBRARY_PATH=$(DEBUG_LIB_DIR):$(CRUX_DEBUG_LIB_DIR) ./$(TEST_SHARED_BIN_DEBUG)

# Run main programs
run_main_release_static: main_release
	$(MAIN_STATIC_BIN_RELEASE)

run_main_release_shared: main_release
	LD_LIBRARY_PATH=$(RELEASE_LIB_DIR):$(CRUX_RELEASE_LIB_DIR) ./$(MAIN_SHARED_BIN_RELEASE)

run_main_debug_static: main_debug
	$(MAIN_STATIC_BIN_DEBUG)

run_main_debug_shared: main_debug
	LD_LIBRARY_PATH=$(DEBUG_LIB_DIR):$(CRUX_DEBUG_LIB_DIR) ./$(MAIN_SHARED_BIN_DEBUG)

format:
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +

# Clean build artifacts
clean:
	rm -rf $(LIB_DIR) $(TEST_BIN_DIR) $(BIN_DIR)

.PHONY: all release debug test_release test_debug main_release main_debug clean \
        run_test_release_static run_test_release_shared run_test_debug_static run_test_debug_shared \
        run_main_release_static run_main_release_shared run_main_debug_static run_main_debug_shared
