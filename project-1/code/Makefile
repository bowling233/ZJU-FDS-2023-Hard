CC = gcc
CFLAGS = -O3 -Wall -Wextra -Werror -pedantic-errors
TEST_FILES = $(wildcard data/*.txt)

all: gen_data run

gen_data: build
	mkdir -p data
	./gen 5 5
	./gen 10 5
	./gen 30 5
	./gen 50 5
	./gen 80 5
	./gen 100 5

build: mss.c mss.h main.c gen.c
	$(CC) $(CFLAGS) -o mss mss.c main.c
	$(CC) $(CFLAGS) -o gen gen.c mss.c

run: build
	for file in $(TEST_FILES); do \
		echo "Running $$file"; \
		./mss $$file 1 >> $$file.1out; \
		./mss $$file 2 >> $$file.2out; \
		./mss $$file 3 >> $$file.3out; \
		diff3 $$file.1out $$file.2out $$file.3out; \
	done

clean:
	rm -rf data
	rm -f mss
	rm -f report.csv
	rm -f gen

.PHONY: all clean gen_make