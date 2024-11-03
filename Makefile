# Variables
SRCDIR = src
OBJDIR = obj
DEPDIR = dep
BINDIR = bin
TESTDIR = tests
BINTESTDIR = $(BINDIR)/tests
INCDIR = include
EXEC = mini_projet

CC = gcc
CPPFLAGS = -I$(INCDIR)
CFLAGS = -Wall -Wextra -ansi -pedantic -std=gnu99
LDFLAGS = -Llib -lm

# List source and object files
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
TESTS = $(wildcard $(TESTDIR)/*.c)
TEST_BIN = $(TESTS:$(TESTDIR)/%.c=$(BINTESTDIR)/%)

# For debugging purpose
$(info SRC = $(SRC))
$(info OBJ = $(OBJ))
$(info DEP = $(DEP))

# Flags to manage dependencies
CPPFLAGS += -MMD -MP

# Default target
all: prepare $(BINDIR)/$(EXEC)

# Create directories if needed
prepare:
	mkdir -p $(OBJDIR) $(DEPDIR) $(BINDIR) $(BINTESTDIR)

# Compile the application (linking)
$(BINDIR)/$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# Rule to generate object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Include dependencies
-include $(DEP)

# Rule for the tests
tests: prepare $(TEST_BIN)

$(BINTESTDIR)/%: $(TESTDIR)/%.c $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(filter-out $(OBJDIR)/main.o, $(OBJ)) -o $@ $(LDFLAGS)

# Run the tests
run-tests: tests
	@for test in $(TEST_BIN); do \
		echo "Running $$test..."; \
		./$$test; \
	done

# Run the application
run: all
	./$(BINDIR)/$(EXEC)

# Cleaning
clean:
	rm -rf $(OBJDIR)/*.o $(DEPDIR)/*.d $(BINTESTDIR)/*
	find . -name '.DS_Store' -type f -delete
	rm -rf $(OBJDIR) $(DEPDIR) || true

# Distclean with forced removal of files
distclean: clean
	rm -rf $(BINDIR)/$(EXEC)
	@if [ -d $(BINTESTDIR) ]; then \
		rm -rf $(BINTESTDIR)/*; \
		rmdir $(BINTESTDIR); \
	fi
	@if [ -d $(BINDIR) ]; then \
		rm -rf $(BINDIR)/*; \
		rmdir $(BINDIR); \
	fi

.PHONY: all clean distclean prepare tests run-tests run
