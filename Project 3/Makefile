# Makefile for building and testing AVL_Database project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Target executable
TARGET = AVL_Database

# Source files
SOURCES = AVL_Database.cpp db_driver.cpp

# Build target
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET)
