CC = gcc
CC_FLAGS = -Wall -pedantic -std=c18

SRC_FOLDER= ./src
SRC_MAIN = hashtables.c
SRC= $(SRC_FOLDER)/$(SRC_MAIN)

BIN_FOLDER = ./build
BIN = $(BIN_FOLDER)/hashtables.bin
DEBUG_BIN = $(BIN_FOLDER)/hashtables-debug.bin

.PHONY=default build run clean valgrind

default: clean run

build: $(SRC)
	mkdir -p $(BIN_FOLDER)
	$(CC) $(SRC) $(CC_FLAGS) -o $(BIN)

run: build
	$(BIN)

clean:
	rm -rf $(BIN_FOLDER)

valgrind: $(SRC)
	mkdir -p $(BIN_FOLDER)
	gcc $(SRC) $(CC_FLAGS) -g -ggdb -O0 -o $(DEBUG_BIN)
	valgrind $(DEBUG_BIN)
