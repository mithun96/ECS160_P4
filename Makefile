# Target program
program := maxTweeter.out

# Default rule
all: $(program)

# Print the commands unless explicitely requested with `make V=1`
ifneq ($(V),1)
Q = @
V = 0
endif

# Current directory
CUR_PWD := $(shell pwd)

# Compilation toolchain
CC	= $(CUR_PWD)/afl/afl/afl-gcc #gcc

# gcc options
CFLAGS	:= -Werror
CFLAGS  += -ansi
CFLAGS  += -pedantic
CFLAGS	+= -g

# Rule for maxTweeter.out
$(program): maxTweeter.o
	@echo "CC	$@"
	$(Q)$(CC) $(CFLAGS) -o $@ $<
	@echo "RM	$<"
	@rm -f $<

# Generic rule for compiling objects
%.o: %.c
	@echo "CC	$@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

# Cleaning rule
clean:
	@echo "CLEAN	$(CUR_PWD)"
	$(Q)rm -f $(program) *.o
