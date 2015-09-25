// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"

#include "string.h"
#include "set.h"

// #include "Map.h" ... if you decide to use the Map ADT

typedef int encounter_type;

#define VAMPIRE_HATCHLING 0
#define TRAP              1
#define MAX_ENCOUNTERS    3
#define SIZE_OF_TURN      8
#define EXPIRY_INDEX      5
#define FINAL_TRAIL_IND   5
#define TRAP_IND          3
#define VAMP_IND          4
//#define DOUBLE_BACK_CONVERSION 102

struct dracView {

    // Stores the current round
    int round;

    // Stores the current player
    int currentPlayer;

    // Stores the current score
    int score;
    
    // Arrays to store data for each player 
    // Individual arrays for each of size 5;(0,1,2,3 = hunters), 4=drac

    // Stores the health
    int health[NUM_PLAYERS];

    // Stores the current location
    int location[NUM_PLAYERS];

    // Stores the traps and vamps
    int locationTraps[NUM_MAP_LOCATIONS];
    int locationVamps[NUM_MAP_LOCATIONS];

    // Stores the Drac trail
    LocationID dracTrail[TRAIL_SIZE];

    // Stores the game view
    GameView game;
};



// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{

    // Create the new dracView
    DracView dracView = malloc(sizeof(struct dracView));
    
    // Set basics from gameview: round, current player, score, health, location and dracula's trail
    GameView gv;
    gv = newGameView(pastPlays, messages);
    dracView->game = gv;
    dracView->round = getRound(gv);
    dracView->currentPlayer = getCurrentPlayer(gv);
    dracView->score = getScore(gv);
    getHistory(gv,PLAYER_DRACULA,dracView->dracTrail);
    
    // Variables for looping and such
    int i,j,a;

    // Stores the location which just dropped out of the trail
    LocationID oldestLocation;

    // The current player
    int currPlayer = PLAYER_LORD_GODALMING;

    // Set the current location and health of the players
    for (a = 0; a < NUM_PLAYERS; a++ ) {
        dracView->health[a] = getHealth(gv,a);
        dracView->location[a] = getLocation(gv,a);
    }

    // Set all the traps and vamps to 0
    for (i=0;i<NUM_MAP_LOCATIONS;i++) {
        dracView->locationTraps[i] = 0;
        dracView->locationVamps[i] = 0;
    }

    // Get the size of the pastPlays string (+ 1 for an extra space)
    int pastPlaysSize = strlen(pastPlays) + 1;
    
    // If it's empty, just return the empty game view
    if (pastPlaysSize == 1) {
        return dracView;
    }

    // Stores all of Draculas moves (for traps)
    LocationID dracMoves[GAME_START_SCORE];
    LocationID dracMovesUpto = 0;

    // Loop to store locations of all traps and vamps
    for(i=0;i<pastPlaysSize;i=i+SIZE_OF_TURN) {

        // Get the current location
        char currLocation[2];
        currLocation[0] = pastPlays[i+1];
        currLocation[1] = pastPlays[i+2];

        // Get the current location id
        LocationID currLocationID = abbrevToID(currLocation);

        if(currPlayer == PLAYER_DRACULA) {

            // Add the location to the array storing all his moves
            dracMoves[dracMovesUpto] = currLocationID;
            dracMovesUpto++;

            // Check for placement of traps and vamp hatchlings
            for (j=3;j<5;j++) {
                if (pastPlays[i+j] == 'T') {

                    // Place trap in current location
                    dracView->locationTraps[currLocationID]++;
                } else if (pastPlays[i+j] == 'V') {

                    // Place vamp hatchling in current location
                    dracView->locationVamps[currLocationID]++;
                }
            }

            if (dracMovesUpto > 6) {

                // Get the oldest location
                oldestLocation = dracMoves[dracMovesUpto-7];

                // Check for expiration for traps and vamp hatchlings
                if (pastPlays[i+5] == 'M') {

                    dracView->locationTraps[oldestLocation]--;

                    

                    // Remove oldest vamp
                    // for (j=5;j!=0;j--) {
                    //     if (dracView->dracTrail[j] != -1) {
                    //         if (dracView->locationVamps[dracView->dracTrail[j]] > 0) {
                    //             dracView->locationVamps[dracView->dracTrail[j]]--;
                    //             break;
                    //         }
                    //     }
                    // }
                } else if (pastPlays[i+5] == 'V') {

                    dracView->locationVamps[oldestLocation]--;

                    // Remove oldest trap
                    // for (j=5;j!=0;j--) {
                    //     if (dracView->dracTrail[j] != -1) {
                    //         if (dracView->locationTraps[dracView->dracTrail[j]] > 0) {
                    //             dracView->locationTraps[dracView->dracTrail[j]]--;
                    //             break;
                    //         }
                    //     }
                    // }
                }
            }
        } else {

            // Check for traps and vamp hatchling encounters
            for (j=3;j<7;j++) {
                if (pastPlays[i+j] == 'T') {
                    printf("Trap encountered at %d\n", currLocationID);

                    // Remove trap in current location
                    dracView->locationTraps[currLocationID]--;
                } else if (pastPlays[i+j] == 'V') {

                    // Remove vamp in current location
                    dracView->locationVamps[currLocationID]--;
                }
            }
        }

        // Move onto the next player
        currPlayer = (currPlayer+1)%NUM_PLAYERS;
    }
    return dracView;
}


// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    return currentView->round;
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    return currentView->score;
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    return currentView->health[player];
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    return getLocation(currentView->game, player);
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
              LocationID *start, LocationID *end)
{
    LocationID trail[TRAIL_SIZE];
    getHistory(currentView->game, player, trail);

    *start = trail[1];
    *end = trail[0];
    
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                int *numTraps, int *numVamps)
{
    *numTraps = currentView->locationTraps[where];
    *numVamps = currentView->locationVamps[where];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE])
{
    // Will show Dracula's location (if not a special move) due to the way
    // the gameView is implemented, still need to check for special moves
    getHistory(currentView->game, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    
    /*THINGS TO NOTE: no train,
     cannot be on trail,
     only one hide and one double back at one time,
     no hospital ever,
     hide: same city CANNOT HIDE AT SEA,
     double back: can go back to something on trail  */

    return whereCanTheyGo(currentView, numLocations, PLAYER_DRACULA,
                           road, 0, sea);
    
    // LocationID *where = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
    // int i, num;

    // where[0] = currentView->location[PLAYER_DRACULA];

    // num = 1;
    
    // //first case, first move
    
    // if(getRound(currentView->game)== 0) {
    //     //move anywhere except hospital
    //     for(i=0; i<NUM_MAP_LOCATIONS; i++) {
    //         if(i != ST_JOSEPH_AND_ST_MARYS) {
    //             where[i] = i;
    //         }
    //     }
    //     num = NUM_MAP_LOCATIONS -1;
    // }
    
    // else {
    //     //first find whether dobule backed or not, hid or not
    //     int hideState = FALSE;
    //     int doubleState = FALSE;
    //     //LocationID *dracTrail;
    //     getHistory(currentView->game, PLAYER_DRACULA, currentView->dracTrail); // Is that allowed???
    //     int j;
    //     for (j=0; j<TRAIL_SIZE; j++){
    //         if(currentView->dracTrail[j] == HIDE){
    //             hideState = TRUE; //hidden
    //         }
    //         if( (currentView->dracTrail[j] <= DOUBLE_BACK_5) && (currentView->dracTrail[j] >= DOUBLE_BACK_1)){
    //             doubleState = TRUE;  //double backed
    //             //int doubleNum = dracTrail[j] - DOUBLE_BACK_CONVERSION; //converts to 1-5
    //         }
    //     }
        
    //     //add all adjacent except those on  trail
    //     //first get adjacents
    //     int tempNum;
    //     LocationID *adjacent = connectedLocations(currentView->game, &tempNum,
    //                                               whereIs(currentView, PLAYER_DRACULA),
    //                                               PLAYER_DRACULA,
    //                                               getRound(currentView->game), road, FALSE, sea);
    //     //copy to the adjacent array elements that are not on the trail
    //     int whereCounter;
    //     int k;
    //     for(whereCounter=0; whereCounter < tempNum; whereCounter++) { //for each adjacent
    //         int isInTrail = FALSE;
    //         for (k=0; k<TRAIL_SIZE; k++) { //check if its in trail
    //             if (currentView->dracTrail[k] == adjacent[whereCounter]) {
    //                 isInTrail = TRUE;
    //             }
    //         }Location: Budapest

    //         if(isInTrail == FALSE) {
    //             where[num] = adjacent[whereCounter];
    //             num++; //only increases if not in trail
    //         }
    //     }
    //     //if doubleBack not true, add all locations on trail except current

    //     if (doubleState == FALSE){
    //         int l;
    //         for(l = 1; l<TRAIL_SIZE; l++) { //skip ind 0
    //             where[num] = currentView->dracTrail[l];
    //             num++;
    //         }
        
    //     }
        
    //     //if hide not true, add current location unless currently at sea
    //     if ((hideState == FALSE) && (idToType(currentView->dracTrail[0] != SEA)) ){
    //         where[num] = currentView->dracTrail[0];
    //         num++;
    //     }
    // }
    
    // *numLocations = num;
    // return where;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    LocationID *where = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
    LocationID *dracWhere = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
    int i = 0;
    int j;
    
    if(player == PLAYER_DRACULA) {
        where = connectedLocations(currentView->game, &i,
                                   whereIs(currentView, player),PLAYER_DRACULA,
                                   currentView->round, road, 0, sea);
        
        // Need to truncate any cities already in his trail

        // Create a city with his trail
        Set s = newSet();

        // Set k to 0 (will store new length of where he can go)
        int k = 0;

        // Make a set storing his trail
        for (j=0;j<TRAIL_SIZE;j++) {
            if (currentView->dracTrail[j] != -1) {
                insertInto(s, idToName(currentView->dracTrail[j]));
            }
        }

        // For any city already in his trail, don't add it to where he can go
        for (j=0;j<i;j++) {
            if (!isElem(s, idToName(where[j]))) {
                dracWhere[k] = where[j];
                k++;
            }
        }

        // Return it
        *numLocations = k;
        return dracWhere;

    }
    
    else {
        where = connectedLocations(currentView->game, &i,
                                   whereIs(currentView, player),player,
                                   currentView->round, road, rail, sea);
    }

    *numLocations = i;
    return where;
}