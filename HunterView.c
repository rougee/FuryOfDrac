// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
// #include "Map.h" ... if you decide to use the Map ADT

// Extra includes
#include <stdio.h>
#include "Map.h"
#include "set.h"

struct hunterView {

    // Stores the current round
    int round;

    // Stores the current player
    int currentPlayer;

    // Stores the current score
    int score;

    //Individual arrays for each of size 5;(0,1,2,3 = hunters)4=drac

    // Stores the health
    int health[NUM_PLAYERS];

    // Stores all the moves made in a 2d array (current location stored in corresponding upto)
    int path[NUM_PLAYERS][GAME_START_SCORE*4];
    int upto[NUM_PLAYERS];

    // Stores the trail of the playesrs
    int trail[NUM_PLAYERS][TRAIL_SIZE]; 

    // Stores the game view
    GameView gameView;
};


// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{    
    // Initialise the views
    GameView gv = newGameView(pastPlays, messages);
    HunterView hunterView = malloc(sizeof(struct hunterView));
    
    // Initialised needed variables
    int temptrail[TRAIL_SIZE];
    int i, j;
    
    // Set some of the data using the gameview
    hunterView->gameView = gv;
    hunterView->score = getScore(gv);
    hunterView->round = getRound(gv);
    hunterView->currentPlayer = getCurrentPlayer(gv);    

    // Set the health of all the players
    for (i = 0; i < NUM_PLAYERS ; i++){
        hunterView->health[i] = getHealth(gv, i);
    }

    // Set the trail of all the players
    for (i=0; i < NUM_PLAYERS ; i++){
        getHistory(gv, i, temptrail);
        for(j=0 ; j < TRAIL_SIZE ; j++){
            hunterView->trail[i][j] = temptrail[j];
        }
    }
    
    return hunterView;
}


// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    return currentView->round;
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    return currentView->currentPlayer;
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    return currentView->score;
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    return currentView->health[player];
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    return getLocation(currentView->gameView, player);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player, LocationID trail[TRAIL_SIZE])
{
    int i = 0;  //Counter

    // Loop through currentView trail and fill given trail
    for (i = 0; i < TRAIL_SIZE; i++) {
        trail[i] = currentView->trail[player][i];
    }
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations, int road, int rail, int sea)
{
    PlayerID player = whoAmI(currentView);
    LocationID from = whereIs(currentView, player);
    Round round = giveMeTheRound(currentView);
    return connectedLocations(currentView->gameView, numLocations, from, player, round, road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations, PlayerID player, int road, int rail, int sea)
{
    LocationID from = whereIs(currentView,player);
    Round round = giveMeTheRound(currentView);
    return connectedLocations(currentView->gameView, numLocations, from, player, round, road, rail, sea);
}
