CC=gcc
CFLAGS=-std=c99 -pedantic -Wall
CFLAGS+=-Iinclude
LDFLAGS=-fsanitize=address
DBGFLAGS=-g -DDEBUG=1

DBG=0
OUT=build
BIN=pedestrians
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,$(OUT)/%.o,$(SRC))

ifeq ($(DBG), 1)
	CFLAGS+=$(DBGFLAGS)
endif

.PHONY: build run clean

build: dirs compile

dirs:
	mkdir -p $(OUT)

compile: $(OBJ)
	$(CC) -o $(OUT)/$(BIN) $^ $(LDFLAGS)

$(OUT)/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

run: build
	$(OUT)/$(BIN)

clean:
	rm -f $(OUT)/*.o
	rm -f $(OUT)/$(BIN)
