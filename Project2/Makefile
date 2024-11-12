# Makefile for building and testing FileSystem project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11

# Target executable
TARGET = filesystem

# Source files
SOURCES = FileSystem.cpp FileSystemTester.cpp

# Build target
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET)