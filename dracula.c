// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

#include "Globals.h"
#include "Places.h"


#define TRUE  1
#define FALSE 0

void decideDraculaMove(DracView gameState)
{
	// Create the move character (make default GE)
	char *move = "GE";

	// Get the possible moves
	int numberOfPossibleMoves;
	LocationID *possibleMoves = (LocationID*)whereCanIgo(gameState, &numberOfPossibleMoves, 1, 1);

	// Get Dracula's trail
	LocationID trail[TRAIL_SIZE];
	giveMeTheTrail(gameState, PLAYER_DRACULA, trail);

	int i, j;

	if (numberOfPossibleMoves == 0 && giveMeTheRound(gameState) == 0) {

		// Leave the move as the default location (start of game)

	} else if (numberOfPossibleMoves == 0) {
		// If there are no valid moves, teleport
		move[0] = 'T';
		move[1] = 'P';
	} else {

		// Otherwise, move randomly
		int canMove = FALSE;
		int inTrail;

		// Start at 0, and keep going until the move isn't in Dracula's trail
		LocationID currLocation;

		// For each possible move, if it's not in the trail, set the move as that
		for (i=0;i<numberOfPossibleMoves;i++) {

			inTrail = FALSE;
			currLocation = possibleMoves[i];


			for (j=0;j<TRAIL_SIZE;j++) {
				if (currLocation == trail[j]) {
					inTrail = TRUE;
				}
			}

			
			// Set the move if not in the trail and break
			if (!inTrail) {
				move = idToAbbrev(possibleMoves[i]);
				canMove = TRUE;
				break;
			}

		}


		if (!canMove) {
			// No valid mvoes, so teleport
			move = "TP";
		}
	}
	
	registerBestPlay(move,"");
}
