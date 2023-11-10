CC = cc
CC_FLAGS = 

SRC_FOLDER= ./src
SRC_MAIN = hashtables.c
SRC= $(SRC_FOLDER)/$(SRC_MAIN)

BIN_FOLDER = ./build
BIN = $(BIN_FOLDER)/hashtables.bin

.PHONY=default build run clean

default: clean run

build: $(SRC)
	mkdir -p $(BIN_FOLDER)
	$(CC) $(SRC) `$(CC_FLAGS)` -o $(BIN)

run: build
	$(BIN)

clean:
	rm -rf $(BIN_FOLDER)
