
CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LFLAGS = -lm
HEADERS = mat_utils.h symnmf.h utils.h

.PHONY: all clean

all: symnmf

symnmf: symnmf.o mat_utils.o utils.o
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

symnmf.o: symnmf.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

mat_utils.o: mat_utils.c mat_utils.h
	$(CC) $(CFLAGS) -c $<

utils.o: utils.c utils.h mat_utils.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o symnmf

