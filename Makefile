CC := gcc
CCFLAGS := -Wall -g

SRC_PATH := ./src
BIN_DIR := $(SRC_PATH)/bin

PROJECT_NAME := matrixc
LIBS := ncurses

BIN_PATH := $(BIN_DIR)/$(PROJECT_NAME)

makedir:
	@mkdir -p $(BIN_DIR)

build: makedir
	$(CC) $(CCFLAGS) $(SRC_PATH)/main.c \
				$(addprefix -l,$(LIBS)) \
				-o $(BIN_PATH)

clean:
	@rm $(BIN_PATH)

run:
	$(BIN_PATH)

install:
	@echo "NOT IMPLEMENTED"
