CC=gcc
CFLAGS=-Iinclude -Wall -Wextra -pedantic -O2
DFLAGS=-Iinclude -Wall -Wextra -pedantic -g
LFLAGS=-lm -lncurses

BIN=jogo
#BIN=play

SRC_DIR=src
OBJ_DIR=obj
DOC_DIR=doc

SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all $(BIN) install debug doc clean

all: $(BIN)

$(BIN): $(OBJECTS) $(OBJ_DIR)/main.o
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@

install:
	sudo apt-get install libncurses5-dev libncursesw5-dev gdb valgrind doxygen graphviz || exit 1

debug: $(OBJECTS) $(OBJ_DIR)/main.o
	$(CC) $^ $(DFLAGS) $(LFLAGS) -o $@

doc:
	doxygen $(DOC_DIR)/Doxyfile
	rm -rf $(DOC_DIR)/html
	mv html $(DOC_DIR)/

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN) debug $(OBJ_DIR)/*.o
