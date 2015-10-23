// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include "dracula.h"

#include <time.h>
#include "Globals.h"
#include "Places.h"
#include "Map.h"


#define TRUE  1
#define FALSE 0

#define MAX_WEIGHT 9999

typedef int bool;

typedef struct _moveState *moveState;

struct _moveState {
   LocationID  move;   // The move
   int         weight; // It's weighting
} _moveState;

// Gets the best move
LocationID getBestMove(moveState *moves, int length, int smallestWeight);

// Checks to see if there is a hide in the trail
bool isHideInTrail(LocationID trail[]);

// Checks to see if there is a double back in the trail
int isDoubleBackInTrail(LocationID trail[]);

// Get's a double back move (assuming it's able to)
char *getDoubleBack(DracView gameState, LocationID fullTrail[]);

// Weighs the map according to hunter positions
void weighMap(int weightedMap[], DracView gameState);

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

    // Get Dracula's full trail (no special moves)
    LocationID fullTrail[TRAIL_SIZE];
    giveMeTheFullTrail(gameState, PLAYER_DRACULA, fullTrail);

    // Variables for looping
    int i, j;

    // Declare the weighted map (default of 5)
    int weightedMap[NUM_MAP_LOCATIONS];
    for (i=0;i<NUM_MAP_LOCATIONS;i++) {
        weightedMap[i] = 5;
    }

    // Weight the map
    weighMap(weightedMap, gameState);

    int l;
    LocationID *s = shortestPath(GRANADA, ROME, weightedMap, &l,1,1,1);


    for (i=0;i<l;i++) {
        printf("%s->", idToName(s[i]));
    }
    printf("end\n");

    // Other variables needed
    int inTrail;
    int smallestWeight = MAX_WEIGHT;

    // Declare an array of moves states, to store the possible moves to choose from
    moveState *moveChoices = calloc(NUM_MAP_LOCATIONS, sizeof(moveState));
    int moveChoicesNum = 0;

    // The current possible move
    moveState currMove;
    LocationID currLocation;

    // The number of legitimate views
    numberOfActualMoves = 0;

    // Populate the moveChoices array with all the possible moves (excluding special moves)
    for (i=0;i<numberOfPossibleMoves;i++) {

        // Filter out cities in trails
        inTrail = FALSE;
        currLocation = possibleMoves[i];

        for (j=0;j<TRAIL_SIZE-1;j++) {
            if (currLocation == trail[j]) {
                inTrail = TRUE;
            }
        }

        
        // If the move is not in the trail
        if (!inTrail) {

            // Increment the actual number of moves
            numberOfActualMoves++;

            // Add it to the array of possible moves and weigh it 1
            currMove = malloc(sizeof(_moveState));
            currMove->move = currLocation;
            currMove->weight = weightedMap[currLocation];
            moveChoices[moveChoicesNum] = currMove;
            moveChoicesNum++;

            // // If it's a sea, weigh it 2
            // if (idToType(currLocation) == SEA) {
            //     currMove->weight = 2;
            // }

            // // If it's the location of a hunter, weigh it 3
            // if (whereIs(gameState, PLAYER_LORD_GODALMING) == currLocation ||
            //     whereIs(gameState, PLAYER_DR_SEWARD) == currLocation ||
            //     whereIs(gameState, PLAYER_VAN_HELSING) == currLocation ||
            //     whereIs(gameState, PLAYER_MINA_HARKER) == currLocation) {
            //     currMove->weight = 3;
            // }

            // // Set it as the smallest weight if it is
            // if (currMove->weight < smallestWeight) {
            //     smallestWeight = currMove->weight;
            // }
        }
    }

    for (i=0;i<numberOfActualMoves;i++) {
        if (weightedMap[i] < moveChoices[i]->weight) {
            smallestWeight = moveChoices[i]->weight;
        }
    }

    // If there are no valid moves and it is round 0, then Dracual can 
    // go wherever
    if (numberOfActualMoves == 0 && giveMeTheRound(gameState) == 0) {

        // Leave the move as the default location (start of game)

    } else if (numberOfActualMoves == 0) {
        // If there are no valid moves, try hide or double back

        // for (i=0;i<TRAIL_SIZE;i++) {
        //     if (trail[i] > 0 && trail[i] < 70) {
        //         printf("%s->", idToName(trail[i]));
        //     } else {
        //         printf("%d->", trail[i]);
        //     }
        // }
        //printf("\n");

        // If there is no hide in the trail, hide
        if (!isHideInTrail(trail)) {
            move = "HI";
        } else if (!isDoubleBackInTrail(trail)) {

            // Otherwise attempt a doubleback
            move = getDoubleBack(gameState, fullTrail);
        } else {

            // Otherwise teleport
            move = "TP";
        }
    } else {

        // Otherwise we select a move from the smallest weighted moves
        move = idToAbbrev(getBestMove(moveChoices, moveChoicesNum, smallestWeight));

        if (idToType(abbrevToID(move)) == SEA) {
            if (!isDoubleBackInTrail(trail)) {
                move = getDoubleBack(gameState, fullTrail);
            }
        }
        
        // Free the array
        for (i=0;i<moveChoicesNum;i++) {
            free(moveChoices[i]);
        }
        free(moveChoices);
    }

    // Register the move
    registerBestPlay(move,"");
}

// Gets the best move
LocationID getBestMove(moveState *moves, int length, int smallestWeight) {

    // Declare an empty array to store the moves with the smallest weights
    LocationID *smallestWeightMoves = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
    int i, index;
    int num = 0;

    // Get random number
    srand(time(NULL));
    int r = rand();
    printf("Smallest weighted is %d\n", smallestWeight);

    // Populate the array with the smallest weighted moves
    for (i=0;i<length;i++) {
        printf("Move is %s, with weight %d\n", idToName(moves[i]->move), moves[i]->weight);
        if (moves[i]->weight <= smallestWeight) {
            smallestWeightMoves[num] = moves[i]->move;
            //printf("Suitable move of %s with weight %d\n", idToName(moves[i]->move), moves[i]->weight);
            num++;
        }
    }

    // Set a random index, and make it the move
    index = r%num;
    LocationID bestMove = smallestWeightMoves[index];

    printf("The best move %d, %d of %d\n", smallestWeightMoves[index], index, num);

    // Free the array
    free(smallestWeightMoves);

    return bestMove;
}

// Checks to see if there is a hide in the trail
bool isHideInTrail(LocationID trail[]) {
    int i;
    for (i=0;i<TRAIL_SIZE-1;i++) {
        if (trail[i] == HIDE) {
            return TRUE;
        }
    }

    return FALSE;
}

// Checks to see if there is a double back in the trail
int isDoubleBackInTrail(LocationID trail[]) {
    int i;
    for (i=0;i<TRAIL_SIZE-1;i++) {
        if (trail[i] >= DOUBLE_BACK_1 && trail[i] <= DOUBLE_BACK_5) {
            return TRUE;
        }
    }
    return FALSE;
}

// Get's a double back move (assuming it's able to)
char *getDoubleBack(DracView gameState, LocationID fullTrail[]) {
    int i;
    int doubleBack = 2;

    for (i=1;i<TRAIL_SIZE-1;i++) {
        if (isAdjacent(whereIs(gameState, PLAYER_DRACULA), fullTrail[i])) {
            break;
            doubleBack++;
        }
    }

    switch (doubleBack) {
        case 2:
            return "D2";
            break;
        case 3:
            return "D3";
            break;
        case 4:
            return "D4";
            break;
        case 5:
            return "D5";
            break;
        default:
            return "D1";
            break;
    }
}

// Weighs the map according to hunter positions
void weighMap(int weightedMap[], DracView gameState) {
    
    // Create a new queue to store the connected cities
    Queue cities = newQueue();

    // An array for the connected cities
    LocationID *possibleMoves;

    // Variables for looping/other things
    int i, player, numOfConnected;
    LocationID curr;

    // Go through each hunter
    for (player=0;player<NUM_PLAYERS-1;player++) {

        // Weigh the location of the hunter
        weightedMap[whereIs(gameState, player)] += 10;

        // Get the connected locations (just pass in values to make the function happy)
        possibleMoves = (LocationID*)getConnectedLocations(40, &numOfConnected,
                                                           whereIs(gameState, player), 
                                                           player, 1, 1, 1, 1);

        // For each of those connections increment their weights and add it to the queue
        for (i=0;i<numOfConnected;i++) {
            weightedMap[possibleMoves[i]] += 5;
            enterQueue(cities, idToName(possibleMoves[i]));
        }

        // For each of the cities connected to the connected cities, increment their weights
        while (!emptyQueue(cities)) {

            // Get that city
            curr = nameToID(leaveQueue(cities));

            // Get all the possible connected locations
            possibleMoves = (LocationID*)getConnectedLocations(40, &numOfConnected,
                                                               curr, player, 1, 1, 1, 1);

            // For each of these increment their weights
            for (i=0;i<numOfConnected;i++) {
                weightedMap[possibleMoves[i]] += 2;
            }

        }
    }

    // Go through each city, and increment the weight of any seas
    for (i=0;i<NUM_MAP_LOCATIONS;i++) {
        if (idToType(i) == SEA) {
            weightedMap[i] += 1;
        }
    }

    // If castle dracula is close, weigh it better assuming there are no hunters there
    if ((whereIs(gameState, PLAYER_DRACULA) == KLAUSENBURG || 
        whereIs(gameState, PLAYER_DRACULA) == GALATZ) && 
        weightedMap[whereIs(gameState, PLAYER_DRACULA)] < 10) {
            weightedMap[CASTLE_DRACULA] = 3;
    }
}