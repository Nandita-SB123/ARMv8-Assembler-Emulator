CC      ?= gcc
CFLAGS  ?= -std=c17 -g \
    -D_POSIX_SOURCE -D_DEFAULT_SOURCE \
    -Wall -pedantic

# LDFLAGS = -lm  # Add the math library
LDLIBS = -lm


SRCDIR=src
OBJDIR=obj
BINDIR=bin

EMUDIR=$(SRCDIR)/Emulator
ASMDIR=$(SRCDIR)/Assembler

SOLUTIONDIR=armv8_testsuite/solution

# Finds all c files recursively in the SRCDIR
SRCS=$(shell find $(SRCDIR) -name "*.c")
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Binaries to build
BINS=$(BINDIR)/assemble $(BINDIR)/emulate

.PHONY: all clean test

all: $(BINDIR) $(OBJDIR) $(BINS) $(SOLUTIONDIR)
	cp $(BINDIR)/assemble $(BINDIR)/emulate $(SOLUTIONDIR)

# Create BINDIR and OBJDIR if they do not exist
$(BINDIR) $(OBJDIR) $(SOLUTIONDIR):
	mkdir -p $@

# Link the object files
$(BINDIR)/assemble: $(OBJDIR)/parse.o $(OBJDIR)/ldrstr_asm.o $(OBJDIR)/br_asm.o $(OBJDIR)/imm_asm.o $(OBJDIR)/helper.o $(OBJDIR)/assemble.o 
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BINDIR)/emulate: $(OBJDIR)/reg_emu.o $(OBJDIR)/decode.o $(OBJDIR)/br_emu.o $(OBJDIR)/ldrstr_emu.o $(OBJDIR)/imm_emu.o $(OBJDIR)/help.o $(OBJDIR)/emulate.o 
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Creating object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(EMUDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(ASMDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BINS) $(OBJS)
