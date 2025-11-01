CC := gcc
CFLAGS := -std=c11 -O2 -Wall -Wextra -Iinclude
LDFLAGS := -lgmp

BIN_DIR := .bin

VPATH := src

SRCS := main.c $(wildcard src/*.c)
SRCS_BASENAMES := $(notdir $(SRCS))
OBJS := $(patsubst %.c,$(BIN_DIR)/%.o,$(SRCS_BASENAMES))

TARGET := $(BIN_DIR)/main

.PHONY: all run clean

all: $(TARGET)

# link
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# compile rule (VPATH allows finding sources in src/)
$(BIN_DIR)/%.o: %.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ensure bin dir exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: all
# @echo "Running $(TARGET)"
	@./$(TARGET) $(filter-out $@,$(MAKECMDGOALS)) 2>&1 | tee out.txt

clean:
	-rm -rf $(BIN_DIR)

# optional: show variables
print-%:
	@echo '$*=$($*)'