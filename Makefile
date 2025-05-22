# Directories
BUILD_DIR = make
SRC_DIR   = src
INC_DIR   = inc

# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -g -I$(INC_DIR)
LDFLAGS = -lncurses

# List all C source files in src folder
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
# Transform source filenames into corresponding object filenames in the BUILD_DIR
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Define final executable location inside the build directory
TARGET = $(BUILD_DIR)/ctEd

# Default target - builds the executable
all: $(TARGET)

# Create the build directory if it doesn't exist; then build the target
$(TARGET): $(BUILD_DIR) $(OBJ_FILES)
		$(CC) $(CFLAGS) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Rule to create the build directory
$(BUILD_DIR):
		mkdir -p $(BUILD_DIR)

# Compile each source file to an object file in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

# Clean up build materials
clean:
		rm -rf $(BUILD_DIR)

.PHONY: all clean

