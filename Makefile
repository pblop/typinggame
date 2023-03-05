CC:=cc
CFLAGS:=-Wall -Wextra -pedantic -std=c99 -g

typinggame: main.c termio.h util.h termio.o util.o
	$(CC) $(CFLAGS) main.c termio.o util.o -o typinggame

termio.o: termio.c termio.h
	$(CC) $(CFLAGS) -c termio.c 

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c
