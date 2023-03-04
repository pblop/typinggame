CC:=cc
CFLAGS:=-Wall -Wextra -pedantic -std=c99 -g

typinggame: main.c
	$(CC) $(CFLAGS) main.c -o typinggame
