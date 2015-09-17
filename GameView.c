// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
// #include "Map.h" ... if you decide to use the Map ADT

struct gameView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    ////////////////////////////////////////////
    //All information required to "view" the game here
    //Get all this from functions below
    //e.g get round will simply gameview->round,
    int round;
    int currentPlayer;
    int score;
    ////////////////////////////////////////////
    ///////Structs
    //Individual arrays for each of size 5;(0,1,2,3 = hunters)4=drac
    //To access drac's health or location health[DRAC] || health[4]
    int *health
    int *location
    //Not sure on how to store path history/past plays, either as 5 arrays as given
    //or a linked list, we will have to implement our own ll though(easier to keep
    //track of turns)
};


// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = malloc(sizeof(struct gameView));
    gameView->hello = 42;
    return gameView;
}


// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    //Done feel free to change
    return currentView->round;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    //Done feel free to change
    return currentView->currentPlayer;
}

    //Done feel free to change
int getScore(GameView currentView)
{
    //Done feel free to change
    return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    //Done feel free to change
    return health[player];
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}
