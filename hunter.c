// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"

#include "Globals.h"
#include "Places.h"

void decideHunterMove(HunterView gameState)
{
	// Create the move character (make default AM)
	char *move = "AM";
    

	// Check health, if below 5 then rest, otherwise move
	if ((howHealthyIs(gameState, whoAmI(gameState))) < 5) {

		// Stay at their current location
		move = idToAbbrev(whereIs(gameState, whoAmI(gameState)));
	} else {

		// Pick a random location from their connected locations

		// Get the possible moves
		int numberOfPossibleMoves;
		LocationID *possibleMoves = (LocationID*)whereCanIgo(gameState, &numberOfPossibleMoves, 1, 1, 1);

		// If there are no possible moves stick with the default location
		if (numberOfPossibleMoves != 0) {
			// Generate a "random" number using the current player number and round numbers as a seed of sorts
			int randomNum = (whoAmI(gameState) + giveMeTheRound(gameState)) % numberOfPossibleMoves;

			// Set the play to that
			move = idToAbbrev(possibleMoves[randomNum]);
		}
	}

    registerBestPlay(move, "");
}
