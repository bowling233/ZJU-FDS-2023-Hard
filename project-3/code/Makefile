CC = gcc
CFLAGS = -lm -O3 
DEBUGFLAGS = -g -Wall -Wextra -Werror -pedantic-errors
TEST_FILES = $(wildcard testdata/*.in)

all: clean unix

unix: main.c graph.c graph.h
	$(CC) -o main graph.c main.c $(CFLAGS) -DNDEBUG

debug: main.c graph.c graph.h
	$(CC) -o main graph.c main.c $(CFLAGS) $(DEBUGFLAGS) 

run: 
	./main

test: all
	for file in $(TEST_FILES); do \
		echo "Running $$file"; \
		./main $$file > $$file.out 2>&1; \
	done

clean:
	rm -rf main main.dSYM testdata/*.out

.PHONY: all clean unix debug run test