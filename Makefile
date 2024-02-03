
CC = g++
BDIR = build
OUT = circuit
SRCDIR = src
OBJDIR = .obj
EXT = .cpp

LFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
CFLAGS = -g -Wall -Wno-unused -Wunused-function 

# EXECUTION

FILES := $(wildcard $(SRCDIR)/*$(EXT))
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BDIR)/$(OBJDIR)/%.o,$(FILES))

## Main
all: $(OBJS)
	$(CC) $(CFLAGS) -o $(BDIR)/$(OUT) $(OBJS) $(LFLAGS)
	@echo "### EXECUTION ###"
	./$(BDIR)/$(OUT)	
	@echo "### END ###"
	rm -r $(BDIR)
	


## Object
$(BDIR)/$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	mkdir -p $(BDIR)/$(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BDIR)
