CC := gcc
SRCD := src
INCD := include 
TSTD := tests
BIND := bin
BLDD := build

ALL_SRCF := $(shell find $(SRCD) -type f -name "*.c")
ALL_OBJF := $(patsubst $(SRCD)/%.c,$(BLDD)/%.o,$(ALL_SRCF))
ALL_INCF := $(shell find $(INCD) -type f -name "*.h")

WFLAGS := -Wall -Wno-unused-function -Werror -Wextra -Wduplicated-cond -Wduplicated-branches -Wshadow -Wnull-dereference
LTHREAD := -lpthread

CFLAGS += $(WFLAGS) 
CFLAGS += $(LTHREAD)

EXEC := proxy
INC := -I $(INCD)

.PHONY: all setup clean

all: setup $(BIND)/$(EXEC) 

setup: $(BLDD) $(BIND)

$(BLDD):
	mkdir -p $@

$(BIND):
	mkdir -p $@

$(BIND)/$(EXEC): $(ALL_OBJF)
	$(CC) $^ -o $@ 

$(BLDD)/%.o: $(SRCD)/%.c
	$(CC) $(CFLAGS) $(INC) $< -c -o $@ 

clean:
	rm -rf $(BLDD)
	rm -rf $(BIND)


