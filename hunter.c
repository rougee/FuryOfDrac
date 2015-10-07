// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"

#include <time.h>
#include "Globals.h"
#include "Places.h"

void decideHunterMove(HunterView gameState)
{

	// Get random number
	srand(time(NULL));
	int r = rand();
	int i;

	// Create the move character (make default AM)
	char *move = "AM";
   
   //starting locations for each player
   if (giveMeTheRound(gameState) == 0) {
      char *startLocations[] = {"AM", "CN", "ZU", "CD"};
      switch (whoAmI(gameState)) {
         case PLAYER_LORD_GODALMING:
            registerBestPlay(startLocations[0], "NOTHING");
            break;
         case PLAYER_DR_SEWARD:
            registerBestPlay(startLocations[1], "NOTHING");
            break;
         case PLAYER_VAN_HELSING:
            registerBestPlay(startLocations[2], "NOTHING");
            break;
         case PLAYER_MINA_HARKER:
            registerBestPlay(startLocations[3], "NOTHING");
            break;
      }
      return;
   }
   
	// Check health, if below 5 then rest, otherwise move
	if ((howHealthyIs(gameState, whoAmI(gameState))) < 5) {

		// Stay at their current location
		move = idToAbbrev(whereIs(gameState, whoAmI(gameState)));
	} else {

		// Pick a random location from their connected locations

		// Get current location
		LocationID currLocation = whereIs(gameState, whoAmI(gameState));

		// Get the possible moves
		int numberOfPossibleMoves;
		LocationID *possibleMoves = (LocationID*)whereCanIgo(gameState, &numberOfPossibleMoves, 1, 1, 1);

		// Remove the current location from it
		LocationID *movesWithoutCurr = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
		int movesWithoutCurrNum = 0;

		for (i=0;i<numberOfPossibleMoves;i++) {
			if (possibleMoves[i] != currLocation) {
				movesWithoutCurr[movesWithoutCurrNum] = possibleMoves[i];
				movesWithoutCurrNum++;
			}
		}


		// If there are no possible moves stick with the default location
		if (numberOfPossibleMoves != 0) {

			// Set the play to that
			if (movesWithoutCurrNum < 1) {
				move = idToAbbrev(movesWithoutCurr[r%movesWithoutCurrNum]);
			} else {
				move = idToAbbrev(possibleMoves[r%numberOfPossibleMoves]);
			}
		}
	}

    registerBestPlay(move, "");
}
