simple-pong
===========

Single Player Pong

NOTE: THIS HAS ONLY BEEN TESTED ON Ubuntu 8.04.4 LTS. When running on OS X 10.7, 
I found that I needed a fresh terminal session, and needed to launch the game
using an absolute path.

This is a single-player pong game written in C. It was a student project,
but again just shows some simple design techniques I used in creating the game.

=============================

Contents:
pong.c
ponglib.c
ponglib.h
pball.c
pball.h
ppaddle.c
ppaddle.h
pcourt.c
pcourt.h
pjmbtrn.c
pjmbtrn.h
preferee.c
preferee.h

Plan - Contains a design overview

Makefile	- Contains command for building the program.         
"make clean" will clean the application binary
"make pong" will clean and build the pong binary.

./pong to start the game


Game Play:
"k" key to move the paddle up
"m" to move the paddle down
"r" to restart game
"Q" to quit game

When all of the balls are lost, the user will be prompted to either restart
with the 'r' key, or quit with the 'Q' key.
