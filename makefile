TARGET_EXEC := ./target/release/spreadsheet

BUILD_DIR := ./build
SRC_DIRS := ./src
TEST_TARGET := testt
TEST_SRC := ./src/unittest.c
TEST_OBJ := $(BUILD_DIR)/$(SRC_DIRS)/unittest.c.o
TEST_OBJS := $(TEST_OBJ) $(BUILD_DIR)/$(SRC_DIRS)/recalculations.c.o $(BUILD_DIR)/$(SRC_DIRS)/functions.c.o $(BUILD_DIR)/$(SRC_DIRS)/sheet.c.o $(BUILD_DIR)/$(SRC_DIRS)/input_process.c.o
# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(SRC_DIRS)/main.c $(SRC_DIRS)/recalculations.c $(SRC_DIRS)/functions.c $(SRC_DIRS)/sheet.c $(SRC_DIRS)/input_process.c

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d) $(TEST_OBJ:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# The final build step.
$(TARGET_EXEC): $(OBJS)
	$(CXX) -O3 $(OBJS) -o $@ 

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) -O3 $(CPPFLAGS) -c $< -o $@

test: $(TEST_TARGET) 
	./$(TEST_TARGET) 
$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) -O3 $(TEST_OBJS) -o $@ 

report: report.pdf

report.pdf: report.tex
	pdflatex report.tex
# $(TEST_OBJ): $(TEST_SRC)
# 	mkdir -p $(dir $@)
# 	$(CC) -O3 $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)