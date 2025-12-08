# output: shared library (.so) and an executable to run tests

BASE_NAME := engine
TARGET := lib$(BASE_NAME).so

# Directories
BIN_DIR_DEBUG := bin/Debug
BIN_DIR_RELEASE := bin/Release
OBJ_DIR_DEBUG := obj/Debug
OBJ_DIR_RELEASE := obj/Release

# Source files
MAIN_SRC_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES_DEBUG := $(MAIN_SRC_FILES:%.cpp=$(OBJ_DIR_DEBUG)/%.o)
MAIN_OBJ_FILES_RELEASE := $(MAIN_SRC_FILES:%.cpp=$(OBJ_DIR_RELEASE)/%.o)

TESTS_RUNNER := $(BASE_NAME)-tests-runner
TEST_SRC_FILES := $(wildcard tests/*_test.cpp)
TEST_OBJ_FILES := $(TEST_SRC_FILES:.cpp=.o)
DEPENDENCIES_DIR := ../dependencies

# Compiler and flags
CXX := g++
INCLUDES := -I$(DEPENDENCIES_DIR)/freeglut/include \
            -I$(DEPENDENCIES_DIR)/freeimage/include \
            -I$(DEPENDENCIES_DIR)/glm/include

# Debug flags
CXX_FLAGS_DEBUG := -c -fPIC -std=c++20 -g -D_DEBUG $(INCLUDES)
LD_FLAGS_DEBUG := -shared

# Release flags
CXX_FLAGS_RELEASE := -c -fPIC -std=c++20 -O2 $(INCLUDES)
LD_FLAGS_RELEASE := -shared

# Library paths
LIB_PATHS := -L$(DEPENDENCIES_DIR)/freeglut/lib/x64/Release \
             -L$(DEPENDENCIES_DIR)/freeimage/lib/x64/Release
LIBS := -lglut -lGL -lGLU -lFreeImage

# Default target
all: debug release

# Debug build
debug: $(BIN_DIR_DEBUG)/$(TARGET)

$(BIN_DIR_DEBUG)/$(TARGET): $(MAIN_OBJ_FILES_DEBUG)
	@mkdir -p $(BIN_DIR_DEBUG)
	$(CXX) $(LD_FLAGS_DEBUG) $(LIB_PATHS) -o $@ $(MAIN_OBJ_FILES_DEBUG) $(LIBS)
	@echo "Debug $(TARGET) build done!"

# Release build
release: $(BIN_DIR_RELEASE)/$(TARGET)

$(BIN_DIR_RELEASE)/$(TARGET): $(MAIN_OBJ_FILES_RELEASE)
	@mkdir -p $(BIN_DIR_RELEASE)
	$(CXX) $(LD_FLAGS_RELEASE) $(LIB_PATHS) -o $@ $(MAIN_OBJ_FILES_RELEASE) $(LIBS)
	@echo "Release $(TARGET) build done!"

# Debug object files
$(OBJ_DIR_DEBUG)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR_DEBUG)
	$(CXX) $(CXX_FLAGS_DEBUG) -o $@ $<

# Release object files
$(OBJ_DIR_RELEASE)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR_RELEASE)
	$(CXX) $(CXX_FLAGS_RELEASE) -o $@ $<

# Clean
clean:
	@rm -rf bin/ obj/
	@rm -f $(TESTS_RUNNER) $(TEST_OBJ_FILES)
	@echo "Cleaned build directories"

# Install (optional)
install: $(BIN_DIR_RELEASE)/$(TARGET)
	@echo "Installing $(TARGET) to system..."
	# Add installation commands here
	@echo "Install done!"

# Phony targets
.PHONY: all debug release clean install