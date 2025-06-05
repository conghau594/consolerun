CC=gcc
CFLAGS=-I_01ConsoleRun -Itests -DTESTING -std=c99

TEST_SRC=tests/test_main.c _01ConsoleRun/main.c
TEST_BIN=tests/test_main

all:
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_BIN)

.PHONY: clean test
clean:
	rm -f $(TEST_BIN)

test: all
	./$(TEST_BIN)
