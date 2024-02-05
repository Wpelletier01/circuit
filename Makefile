

CXX := g++
CXXFLAGS := -std=c++11 -g -Wall -Wno-unused -Wunused-function
LFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET := circuit
BDIR = build
SRC_DIR := src

# Find all .cpp files in the source directory and its subdirectories
SRCS := $(shell find $(SRC_DIR) -type f -name "*.cpp")

# Define the object files corresponding to each source file
OBJS := $(SRCS:.cpp=.o)


# Default target, build the executable
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LFLAGS)
	@echo "### EXECUTION ###"
	./$(TARGET)	
	@echo "### END ###"


# Rule to compile each source file into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony target to avoid conflicts with file names
.PHONY: all clean
