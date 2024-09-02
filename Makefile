# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -g

# Executable names
TARGET = program
TEST_TARGET = test_module

# Source files
SRCS = main.cpp memory_manager.cpp task.cpp
TEST_SRCS = test.cpp

# Header files
HDRS = config.h memory_manager.h task.h

# Object files
OBJS = $(SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Build the main executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Build the test module
$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

# Compile source files into object files
%.o: %.cpp $(HDRS)
	$(CXX) -c $< -o $@

# Compile test source file
test.o: test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -lpthread

# Clean up
clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET) output.txt

# Run the main program
run: $(TARGET)
	./$(TARGET)

run-test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: clean run run-test all

all: $(TARGET) $(TEST_TARGET)