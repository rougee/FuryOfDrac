// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

#include <time.h>
#include "Globals.h"
#include "Places.h"


#define TRUE  1
#define FALSE 0

typedef moveState *_moveState

struct _moveState {
   LocationID  move;   // The move
   int         weight; // It's weighting
};

void decideDraculaMove(DracView gameState)
{
    
    // Get random number
    srand(time(NULL));
    int r = rand();

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

        LocationID *movesWithoutTrail = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
        int movesWithoutTrailNum = 0;

        LocationID *movesWithoutTrailSea = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
        int movesWithoutTrailSeaNum = 0;

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

                // Add it to list of possible moves
                movesWithoutTrail[movesWithoutTrailNum] = currLocation;
                movesWithoutTrailNum++;

                // If its not a sea, add it to possible moves with exclude seas
                if (idToType(currLocation) != SEA) {
                    movesWithoutTrailSea[movesWithoutTrailSeaNum] = currLocation;
                    movesWithoutTrailSeaNum++;
                }

                canMove = TRUE;
            }

        }


        if (!canMove) {
            // No valid mvoes, so teleport
            move = "TP";
        } else {

            // Try for land move, otherwise move into the sea
            if (movesWithoutTrailNum > 0) {
                move = idToAbbrev(movesWithoutTrail[r%movesWithoutTrailNum]);
            } else {
                move = idToAbbrev(movesWithoutTrailSea[r%movesWithoutTrailSeaNum]);
            }

        }
    }
    
    registerBestPlay(move,"");
}
