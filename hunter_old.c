// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"

#include <time.h>
#include "Globals.h"
#include "Places.h"

#include "Map.h"

#define FALSE 0
#define TRUE  1

typedef int bool;

LocationID *getPossibleMoves(HunterView gameState, int *numOfMoves);

LocationID getRandomMove(HunterView gameState);

LocationID getDraculaLastMove(HunterView gameState);

void decideHunterMove(HunterView gameState)
{
    int i;

    // Create the move character (make default AM)
    char *move = "AM";
   
    //starting locations for each player
    if (giveMeTheRound(gameState) == 0) {
        char *startLocations[] = {"AM", "CN", "ZU", "CD"};
        switch (whoAmI(gameState)) {
            case PLAYER_LORD_GODALMING:
            registerBestPlay(startLocations[0], "");
            break;
        case PLAYER_DR_SEWARD:
            registerBestPlay(startLocations[1], "");
            break;
            case PLAYER_VAN_HELSING:
        registerBestPlay(startLocations[2], "");
            break;
            case PLAYER_MINA_HARKER:
        registerBestPlay(startLocations[3], "");
            break;
        }
        return;
    }

    // Get whether a move should be made or if they all should rest
    //int makeMove = giveMeTheRound(gameState) % 4; // 4 is the magic number here

    // Get dracula's trail
    LocationID dracTrail[TRAIL_SIZE];
    giveMeTheTrail(gameState, PLAYER_DRACULA, dracTrail);

    // Variables for storing draculas last know location
    int dracLocationKnown = FALSE;
    LocationID lastKnownLocation;

    // Attempt to find dracula's last known location
    for (i=0;i<TRAIL_SIZE;i++) {

        // If a city in his trail is defined, that that's his last know location
        if (dracTrail[i] >= MIN_MAP_LOCATION && dracTrail[i] <= MAX_MAP_LOCATION) {
            dracLocationKnown = TRUE;
            lastKnownLocation = dracTrail[i];
            break;
        }
    }

    printf("Player: %d, round %d at location %s\n", whoAmI(gameState), giveMeTheRound(gameState), idToName(whereIs(gameState, whoAmI(gameState))));

    // If his last known location was defined, go to it
    if (dracLocationKnown) {

        printf("Dracula's location is know, %s\n", idToName(dracTrail[i]));

        // Get possible moves
        // int numOfPossibleMoves;
        // LocationID *possibleMoves = getPossibleMoves(gameState, &numOfPossibleMoves);

        // Get shortest path to dracula's location (use anb evenly weighted map)

        // Get the weighted map
        int weightedMap[NUM_MAP_LOCATIONS];

        for (i=0;i<NUM_MAP_LOCATIONS;i++) {
            weightedMap[i] = 1;
        }

        printf("Going from %d to %d\n", whereIs(gameState, whoAmI(gameState)), lastKnownLocation);

        // Get the shortest path
        int numOfShortestPath;
        LocationID *shortestPossiblePath = shortestPath(whereIs(gameState, whoAmI(gameState)), lastKnownLocation, 
                                                        weightedMap, &numOfShortestPath, 1, 
                                                        (giveMeTheRound(gameState) + whoAmI(gameState)) % 4, // This states whether they can move by rail
                                                        1);

        move = idToAbbrev(shortestPossiblePath[1]);

        // If the hunter is at dracula's location, stay there
        if (lastKnownLocation == whereIs(gameState, whoAmI(gameState))) {
            move = idToAbbrev(whereIs(gameState, whoAmI(gameState)));
        }

        printf("Move of %s was made\n", idToName(abbrevToID(move)));

    } else {

        // Get the player's trail
        LocationID playerTrail[TRAIL_SIZE];
        giveMeTheTrail(gameState, whoAmI(gameState), playerTrail);

        // If already rested, and no last known location for dracula, then make a random move
        if (playerTrail[0] == playerTrail[1]) {          

            move = idToAbbrev(getRandomMove(gameState));
            printf("Dracula's location unknown, making random move %s\n", idToName(abbrevToID(move)));
        } else {

            // Collectively rest to get dracula's current location
            move = idToAbbrev(whereIs(gameState, whoAmI(gameState)));

            printf("Time to rest, resting at %s\n", idToName(abbrevToID(move)));
        }

        
    }

    registerBestPlay(move, "");
}

LocationID *getPossibleMoves(HunterView gameState, int *numOfMoves) {

    // Variables for looping
    int i = 0;

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


    *numOfMoves = movesWithoutCurrNum;
    return movesWithoutCurr;
}


LocationID getRandomMove(HunterView gameState) {

    // Get random number
    srand(time(NULL));
    int r = rand();

    // Set the inital move to -1
    LocationID move = -1;

    // Get the possible moves (excluding current location)
    int numOfMoves;
    LocationID *possibleMoves = getPossibleMoves(gameState, &numOfMoves);
    
    // Get a random move from the possible moves
    move = possibleMoves[r%numOfMoves];

    // Free the array
    free(possibleMoves);

    return move;
}