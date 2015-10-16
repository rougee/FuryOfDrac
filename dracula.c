// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

#include <time.h>
#include "Globals.h"
#include "Places.h"
#include "Map.h"


#define TRUE  1
#define FALSE 0

typedef int bool;

typedef struct _moveState *moveState;

struct _moveState {
   LocationID  move;   // The move
   int         weight; // It's weighting
} _moveState;

static LocationID getBestMove(moveState *moves, int length, int smallestWeight) {
    LocationID *smallestWeightMoves = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
    int i, index;
    int num = 0;

    // Get random number
    srand(time(NULL));
    int r = rand();

    for (i=0;i<length;i++) {
        if (moves[i]->weight <= smallestWeight) {
            printf("The move is %d\n", moves[i]->move);
            smallestWeightMoves[num] = moves[i]->move;
            num++;
        }
    }

    index = r%num;

    printf("The best move %d, %d of %d\n", smallestWeightMoves[index], index, num);

    LocationID bestMove = smallestWeightMoves[index];

    free(smallestWeightMoves);
    return bestMove;
}

static bool isHideInTrail(LocationID trail[]) {
    int i;
    for (i=0;i<TRAIL_SIZE;i++) {
        if (trail[i] == HIDE) {
            return TRUE;
        }
    }

    return FALSE;
}

static int isDoubleBackInTrail(LocationID trail[]) {
    int i;
    for (i=0;i<TRAIL_SIZE;i++) {
        if (trail[i] >= DOUBLE_BACK_1 && trail[i] <= DOUBLE_BACK_5) {
            return TRUE;
        }
    }
    return FALSE;
}

void decideDraculaMove(DracView gameState)
{
    // Create the move character (make default GE)
    char *move = "GE";

    // Get the possible moves
    int numberOfPossibleMoves, numberOfActualMoves;
    LocationID *possibleMoves = (LocationID*)whereCanIgo(gameState, &numberOfPossibleMoves, 1, 1);

    // Get Dracula's trail
    LocationID trail[TRAIL_SIZE];
    giveMeTheTrail(gameState, PLAYER_DRACULA, trail);

    LocationID fullTrail[TRAIL_SIZE];
    giveMeTheFullTrail(gameState, PLAYER_DRACULA, fullTrail);

    int i, j;

    // Otherwise, move randomly
    int inTrail;
    int smallestWeight = 9;

    moveState *moveChoices = calloc(NUM_MAP_LOCATIONS, sizeof(moveState));
    int moveChoicesNum = 0;

    moveState currMove;

    // Start at 0, and keep going until the move isn't in Dracula's trail
    LocationID currLocation;

    numberOfActualMoves = 0;

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

            numberOfActualMoves++;

            currMove = malloc(sizeof(_moveState));
            currMove->move = currLocation;
            currMove->weight = 1;
            moveChoices[moveChoicesNum] = currMove;
            moveChoicesNum++;

            // If its not a sea, add it to possible moves with exclude seas
            if (idToType(currLocation) == SEA) {
                currMove->weight = 2;
            }

            if (currMove->weight < smallestWeight) {
                smallestWeight = currMove->weight;
            }
        }
    }

    if (numberOfActualMoves == 0 && giveMeTheRound(gameState) == 0) {

        // Leave the move as the default location (start of game)

    } else if (numberOfActualMoves == 0) {
        // If there are no valid moves, try hide or double back

        for (i=0;i<TRAIL_SIZE;i++) {
            if (trail[i] > 0 && trail[i] < 70) {
                printf("%s->", idToName(trail[i]));
            } else {
                printf("%d->", trail[i]);
            }
        }
        printf("\n");

        if (!isHideInTrail(trail)) {
            move = "HI";
        } else if (!isDoubleBackInTrail(trail)) {
            int doubleBack = 2;

            for (i=1;i<TRAIL_SIZE-1;i++) {
                if (isAdjacent(whereIs(gameState, PLAYER_DRACULA), fullTrail[i])) {
                    break;
                    doubleBack++;
                }
            }

            switch (doubleBack) {
                case 2:
                    move = "D2";
                    break;
                case 3:
                    move = "D3";
                    break;
                case 4:
                    move = "D4";
                    break;
                case 5:
                    move = "D5";
                    break;
                default:
                     move = "D1";
                    break;
                }


        } else {
            move = "TP";
        }
    } else {

        move = idToAbbrev(getBestMove(moveChoices, moveChoicesNum, smallestWeight));
        
        for (i=0;i<moveChoicesNum;i++) {
            free(moveChoices[i]);
        }
        free(moveChoices);
    }

    registerBestPlay(move,"");
}
