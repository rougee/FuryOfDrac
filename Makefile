CC = gcc
CFLAGS = -Wall -Werror -g
BINS = testGameView testHunterView testDracView

all : $(BINS)

testGameView : testGameView.o GameView.o Map.o Places.o set.o queue.o 
testGameView.o : testGameView.c Globals.h Game.h 

testHunterView : testHunterView.o HunterView.o GameView.o Map.o Places.o set.o queue.o 
testHunterView.o : testHunterView.c Map.c Places.h

testDracView : testDracView.o DracView.o GameView.o Map.o Places.o set.o queue.o 
testDracView.o : testDracView.c Map.c Places.h

Places.o : Places.c Places.h
Map.o : Map.c set.c set.h queue.c queue.h Map.h Places.h
GameView.o : GameView.c GameView.h Map.c Map.h
HunterView.o : HunterView.c HunterView.h Map.c Map.h
DracView.o : DracView.c Map.c DracView.h Map.c Map.h
set.o : set.c set.h
queue.o : queue.c queue.h

clean :
	rm -f $(BINS) *.o core

