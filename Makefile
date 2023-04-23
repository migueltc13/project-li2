CC=gcc
CFLAGS=-Iincludes -Wall -Wextra -pedantic -O2
DFLAGS=-Iincludes -Wall -Wextra -pedantic -g
LFLAGS=-lm -lncurses

BIN=play

SRC_DIR=src
OBJ_DIR=obj
DOC_DIR=doc

SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all $(BIN) install debug doc clean

all: clean $(BIN) # TODO remove the clean

$(BIN): $(OBJECTS) $(OBJ_DIR)/main.o
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@

install:
	sudo apt-get install libncurses5-dev libncursesw5-dev gdb valgrind doxygen || exit 1

debug: $(OBJECTS) $(OBJ_DIR)/main.o
	$(CC) $^ $(DFLAGS) $(LFLAGS) -o $@

doc:
	doxygen $(DOC_DIR)/Doxyfile

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(BIN) $(OBJ_DIR)/*.o
