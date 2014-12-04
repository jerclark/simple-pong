#
# makefile for hw4 pong.
# 

CC = gcc -Wall 

clean:
	rm -f pong *.o
	
pong: clean pong.c
	$(CC) pong.c pcourt.c pball.c ppaddle.c pjmbtrn.c preferee.c ponglib.c -lcurses -o pong

pongdebug: clean pong.c
	$(CC) -g pong.c pcourt.c pball.c ppaddle.c pjmbtrn.c preferee.c ponglib.c -lcurses -o pong

