CC = cc
CC_FLAGS = 

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
	$(CC) $(SRC) `$(CC_FLAGS)` -o $(BIN)

run: build
	$(BIN)

clean:
	rm -rf $(BIN_FOLDER)

valgrind:
	mkdir -p $(BIN_FOLDER)
	gcc $(SRC) -Wall -pedantic -g -ggdb -O0 -std=c18 -o $(DEBUG_BIN)
	valgrind $(DEBUG_BIN)
