// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "string.h"

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
    int *health;
    int **path;
    int *upto;
    //Not sure on how to store path history/past plays, either as 5 arrays as given
    //or a linked list, we will have to implement our own ll though(easier to keep
    //track of turns)
};


// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = malloc(sizeof(struct gameView));
    //strlen() length, loop through
    int pastPlaysSize = strlen(pastplays);
    //Round
    gameView->round = pastPlaysSize/40;
    //Current Player
    gameView->currentPlayer = (pastPlaysSize/8)%5;
    int i;
    int curPlayer = 0;
    for (i=0;i<5;i++) {
        gameView->upto[i] = 0;
    }
    for(i=0;i<pastPlaysSize;i+8){
        char *currLocation = {pastPlays[i+1], pastPlays[i+2]};
        strcpy(gameView->path[currPlayer][gameView->upto[curPlayer], currLocation);
        if (curPlayer == PLAYER_DRACULA){
        //Check 7th from end move, if it was a trap update trap array as gone,
        //If it was a vamp, update as matured gameView->score -=gameView->score;

            gameView->score -= 1;
        }else{    //Hunter

        //Death check goes after every encounter.
            if(gameView->health <= 0){
                gameView->score -= 6;
            }
        //Hunter teleported to hospital, but this should be in hospital already.
        //Can't test for teleported, otherwise could simply do if(location==hospital)


        }


        curPlayer = (curPlayer+1)%5;
    }
//If hunter is on 0 next turn they'll be on max=9 lp
//Dracula can go above 40bp, starts at 40

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
