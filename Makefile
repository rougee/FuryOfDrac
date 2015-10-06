# Makefile for "Fury of Dracula": The Hunt

# change these to suit your local C environment
CC = gcc
CFLAGS = -Wall -Werror
# do not change the following line
BINS = dracula hunter
# add any other *.o files that your system requires
# (and add their dependencies below after DracView.o)
# if you're not using Map.o or Places.o, you can remove them
OBJS = GameView.o Map.o Places.o set.o queue.o
# add whatever system libraries you need here (e.g. -lm)
LIBS =

all : $(BINS)

dracula : dracPlayer.o dracula.o DracView.o $(OBJS) $(LIBS)
hunter : hunterPlayer.o hunter.o HunterView.o $(OBJS) $(LIBS)

dracPlayer.o : player.c Game.h DracView.h dracula.h
	$(CC) $(CFLAGS) -DI_AM_DRACULA -c player.c -o dracPlayer.o

hunterPlayer.o : player.c Game.h HunterView.h hunter.h
	$(CC) $(CFLAGS) -c player.c -o hunterPlayer.o

dracula.o : dracula.c Game.h DracView.h set.c set.h queue.c queue.h Map.c Map.h Places.h
hunter.o : hunter.c Game.h HunterView.h set.c set.h queue.c queue.h Map.c Map.h Places.h
Places.o : Places.c Places.h
Map.o : set.c set.h queue.c queue.h Map.c Map.h Places.h
GameView.o : GameView.c Globals.h GameView.h   set.c set.h Map.c Map.h
HunterView.o : HunterView.c Globals.h HunterView.h    set.c set.h Map.c Map.h
DracView.o : DracView.c Globals.h DracView.h  set.c set.h Map.c Map.h
# if you use other ADTs, add dependencies for them here

set.o : set.c set.h
queue.o : queue.c queue.h

clean :
	rm -f $(BINS) *.o core

