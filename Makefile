

CXX := g++
CXXFLAGS := -std=c++11 -g -Wall -Wno-unused -Wunused-function
LFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET := circuit
BDIR = build
SRCDIR = src

SOURCES := $(shell find $(SRCDIR) -name "*.cpp")
OBJECTS := $(patsubst %.c, %.o, $(SOURCES))


all: $(TARGET)


$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $(BDIR)/$@ $(CXXFLAGS) $(LFLAGS)
	
	@echo ######## START EXEC ######## 
	$(BDIR)/$(TARGET)
	@echo ######## END EXEC ######## 


# Clean rule to remove compiled files
clean:
	rm -f $(BDIR)/*

# Phony target to avoid conflicts with file names
.PHONY: all clean
