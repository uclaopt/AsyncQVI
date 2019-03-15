CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
# directory of objective files
BUILDDIR := build
# directory of binary files
BINDIR := bin
# directory of source code
SRCDIR := src
# extension of source file
SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

DEPENDENCY := $(shell find $(BUILDDIR) -type f -name *.d 2>/dev/null)

PROB := $(BINDIR)/test

CFLAGS := -g -std=c++0x -MMD -w 
LIB := -lgfortran -lpthread -lm -ansi
INC := -I include


all: $(PROB)

$(PROB): build/test.o
	@echo " $(CC) $^ -o $(PROB) $(LIB)"; $(CC) $^ -o $(PROB) $(LIB)
	@echo " $(PROB) is successfully built."
	@printf '%*s' "150" | tr ' ' "-"
	@printf '\n'

# Compile code to objective files
###################################
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR) $(BINDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

run:
	./$(PROB) -algo 0 -nthreads 1 -check_step 100000 -style 1 -len_state 80000 -len_action 8 -max_outer_iter 10000000 -max_inner_iter 1 -sample_num_1 1 -sample_num_2 1
##############################################
clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(BINDIR)"; $(RM) -r $(BUILDDIR) $(BINDIR)

-include $(DEPENDENCY)

