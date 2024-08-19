# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.

# Directories
SRC_DIR = .
EXAMPLES_DIR = examples
BUILD_DIR = build

# Source files
LIBRARY_HEADER = $(SRC_DIR)/print.h
EXAMPLE_SOURCES = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_EXECUTABLES = $(patsubst $(EXAMPLES_DIR)/%.c,$(BUILD_DIR)/%,$(EXAMPLE_SOURCES))

# Phony targets
.PHONY: all clean

# Default target
all: $(BUILD_DIR) $(EXAMPLE_EXECUTABLES)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile examples
$(BUILD_DIR)/%: $(EXAMPLES_DIR)/%.c $(LIBRARY_HEADER)
	$(CC) $(CFLAGS) $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Print help information
help:
	@echo "Available targets:"
	@echo "  all    : Build all examples (default)"
	@echo "  clean  : Remove build artifacts"
	@echo "  help   : Show this help message"
