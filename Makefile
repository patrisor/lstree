# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Iinclude

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
OUTPUT := lstree

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
.PHONY: all
all: $(OUTPUT)

# Build target
$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(OUTPUT)

# Run the application
.PHONY: run
run: all
	./$(OUTPUT)