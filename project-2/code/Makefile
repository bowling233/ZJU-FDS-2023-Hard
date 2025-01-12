CC = gcc
CFLAGS = -lm -O3 
DEBUGFLAGS = -DDEBUG -g -Wall -Wextra -Werror -pedantic-errors

all: clean unix

unix: main.c expression.c expression.h
	$(CC) -o expr expression.c main.c $(CFLAGS)

debug: main.c expression.c expression.h
	$(CC) -o expr expression.c main.c $(CFLAGS) $(DEBUGFLAGS) 

clean:
	rm -rf expr expr.dSYM