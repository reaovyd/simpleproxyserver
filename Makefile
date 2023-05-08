CC 	 := gcc
SRCD := src
INCD := include 
TSTD := tests
BIND := bin
BLDD := build

ALL_SRCF := $(shell find $(SRCD) -type f -name "*.c")
ALL_OBJF := $(patsubst $(SRCD)/%.c,$(BLDD)/%.o,$(ALL_SRCF))
ALL_INCF := $(shell find $(INCD) -type f -name "*.h")

TST_OBJF := $(filter-out $(BLDD)/main.o, $(ALL_OBJF))
TST_SRCF := $(shell find $(TSTD) -type f -name "*.c")
TST_INCF := $(shell find $(TSTD) -type f -name "*.h")

WFLAGS := -Wall -Wno-unused-function -Werror -Wextra -Wduplicated-cond -Wduplicated-branches -Wshadow -Wnull-dereference
LTHREAD := -lpthread
PEDANTIC := -Wpedantic

DEBUG_FLAGS := -DDEBUG -g

CFLAGS += $(WFLAGS) 
CFLAGS += $(LTHREAD)
CFLAGS += $(PEDANTIC)

INC := -I $(INCD)

EXEC 	  := proxy
TEST_EXEC := $(EXEC)_tests 


LIB_TEST := -lcriterion 
TST_FLAGS := -I $(TSTD) 

.PHONY: all setup debug clean

all: setup $(BIND)/$(EXEC) $(BIND)/$(TEST_EXEC)  

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all 

setup: $(BLDD) $(BIND)
$(BLDD):
	mkdir -p $@
$(BIND):
	mkdir -p $@

$(BIND)/$(EXEC): $(ALL_OBJF)
	$(CC) $^ -o $@ 

$(BIND)/$(TEST_EXEC): $(TST_OBJF) $(TST_SRCF) $(TST_INCF)
	$(CC) $(TST_FLAGS) $(LIB_TEST) $(INC) $(CFLAGS) $^ -o $@

$(BLDD)/%.o: $(SRCD)/%.c $(ALL_INCF)
	$(CC) $(CFLAGS) $(INC) $< -c -o $@ 

clean:
	rm -rf $(BLDD)
	rm -rf $(BIND)
