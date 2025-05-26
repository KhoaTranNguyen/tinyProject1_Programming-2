# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Target executable
TARGET = main

# Source files
SRCS = main.cpp Vector.cpp

# Header files (not strictly necessary here, but good practice)
HDRS = Vector.hpp

# Build target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Run target
run: $(TARGET)
	./$(TARGET)

# Clean target
clean:
	rm -f $(TARGET)