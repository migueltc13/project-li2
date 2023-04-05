CC=gcc
CFLAGS=-Iincludes -Wall -Wextra -pedantic -O2
DFLAGS=-Iincludes -Wall -Wextra -pedantic -g
LFLAGS=-lm -lncurses

BIN=play

SRC_DIR=src
OBJ_DIR=obj

SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all $(BIN) clean

all: clean $(BIN)

$(BIN): $(OBJECTS) $(OBJ_DIR)/main.o
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@

debug: $(OBJECTS) $(OBJ_DIR)/main.o
	$(CC) $^ $(DFLAGS) $(LFLAGS) -o $@

install:
	sudo apt-get install libncurses5-dev libncursesw5-dev gdb valgrind || exit 1

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN) $(OBJ_DIR)/*.o
