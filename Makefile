CC:=cc
CFLAGS:=-Wall -Wextra -pedantic -std=c99 -g

typinggame: main.c terminput.h termoutput.h terminput.o termoutput.o
	$(CC) $(CFLAGS) main.c terminput.o termoutput.o -o typinggame

terminput.o: terminput.c terminput.h
	$(CC) $(CFLAGS) -c terminput.c 

termoutput.o: termoutput.c termoutput.h
	$(CC) $(CFLAGS) -c termoutput.c
