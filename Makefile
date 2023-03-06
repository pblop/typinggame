CC:=cc
CFLAGS:=-Wall -Wextra -pedantic -std=c99 -g

typinggame: main.c termio.h util.h common.h termio.o util.o
	$(CC) $(CFLAGS) main.c termio.o util.o -o typinggame

termio.o: termio.c termio.h common.h
	$(CC) $(CFLAGS) -c termio.c 

util.o: util.c util.h common.h
	$(CC) $(CFLAGS) -c util.c

run: typinggame
	./typinggame

debug: typinggame
	gdb ./typinggame

