CC:=cc
CFLAGS:=-Wall -Wextra -pedantic -std=c99 -g

typinggame: main.c termio.h termio.o
	$(CC) $(CFLAGS) main.c termio.o -o typinggame

termio.o: termio.c termio.h
	$(CC) $(CFLAGS) -c termio.c 
