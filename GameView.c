// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "string.h"

// Extra includes
#include <stdio.h>
#include "Map.h"


typedef int encounter_type;

#define VAMPIRE_HATCHLING 0
#define TRAP              1

#define SIZE_OF_TURN 8

#define MAX_ENCOUNTERS 3

// #include "Map.h" ... if you decide to use the Map ADT

struct gameView {

    //All information required to "view" the game here
    //Get all this from functions below
    //e.g get round will simply gameview->round,

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

    // Stores all the moves made in a 2d array (current location stored in corresponding upto)
    // The max number of moves is the greatest number of possible rounds
    int path[NUM_PLAYERS][GAME_START_SCORE];
    int upto[NUM_PLAYERS];
};

// Checkes if a given value (c) is in an array(a) upto length n
int isIn(int *a, int n, int c) {
    int i;

    // Loop through the array
    for (i=0;i<n;i++) {

        // If the value is found, return TRUE
        if (a[i] == c) {
            return TRUE;
        }
    }

    // List has been searched, so value not found, so return FALSE
    return FALSE;
}

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    // Create the new gameView
    GameView gameView = malloc(sizeof(struct gameView));
    
    // Get the size of the pastPlays string (+ 1 for an extra space)
    int pastPlaysSize = strlen(pastPlays) + 1;

    // The round will be pastPlaysSize/(size of a turn * number of players)
    gameView->round = pastPlaysSize/(SIZE_OF_TURN*NUM_PLAYERS);

    // The current player will be (pastPlaysSize/size of a turn)%number of players
    gameView->currentPlayer = (pastPlaysSize/SIZE_OF_TURN)%NUM_PLAYERS;

    // Variables for looping/comparison
    int i, j;
    int back;

    // Set the current player (always starts as Lord Godalming)
    int currPlayer = PLAYER_LORD_GODALMING;

    // Set all the upto's to be initially 0 (no moves have been made yet)
    for (i=0;i<5;i++) {
        gameView->upto[i] = 0;
    }

    // Initialise Game score
    gameView->score = GAME_START_SCORE;

    // Initialise hunter's health
    for (i=0;i<NUM_PLAYERS-1;i++) {
        gameView->health[i] = GAME_START_HUNTER_LIFE_POINTS;
    }

    // Initialise Dracula's health
    gameView->health[PLAYER_DRACULA] = GAME_START_BLOOD_POINTS;

    // If it's empty, just return the empty game view
    if (pastPlaysSize == 1) {
        return gameView;
    }

    // A for loop is used to process each turn
    for(i=0;i<pastPlaysSize-7;i=i+SIZE_OF_TURN) {

        // Get the current location by concatenating the second and third character
        char currLocation[2];
        currLocation[0] = pastPlays[i+1];
        currLocation[1] = pastPlays[i+2];

        // Get the current location id
        LocationID currLocationID = abbrevToID(currLocation);

        // Move the current location into the struct

        // If current player is Dracula move might not be a location
        if (currPlayer == PLAYER_DRACULA) {

            // No location, so different move was made, check all special moves
            if (currLocation[0] == 'C' && currLocation[1] == '?') {
                currLocationID = CITY_UNKNOWN;
            } else if (currLocation[0] == 'S' && currLocation[1] == '?') {
                currLocationID = SEA_UNKNOWN;
            } else if (currLocation[0] == 'H' && currLocation[1] == 'I') {
                currLocationID = HIDE;
            } else if (currLocation[0] == 'T' && currLocation[1] == 'P') {
                currLocationID = TELEPORT;
            } else {
                if (currLocation[0] == 'D' && currLocation[1] == '1') {
                    currLocationID = DOUBLE_BACK_1;
                } else if (currLocation[0] == 'D' && currLocation[1] == '2') {
                    currLocationID = DOUBLE_BACK_2;
                } else if (currLocation[0] == 'D' && currLocation[1] == '3') {
                    currLocationID = DOUBLE_BACK_3;
                } else if (currLocation[0] == 'D' && currLocation[1] == '4') {
                    currLocationID = DOUBLE_BACK_4;
                } else if (currLocation[0] == 'D' && currLocation[1] == '5') {
                    currLocationID = DOUBLE_BACK_5;
                }
            }
        }
        gameView->path[currPlayer][gameView->upto[currPlayer]] = currLocationID;

        // Increment the corresponding upto
        gameView->upto[currPlayer] += 1;

        // If the current player is Dracula
        if (currPlayer == PLAYER_DRACULA) {


            // If Dracula is at sea (a see move or in known sea LocationID) he loses 2 health
            if (currLocation[0] == 'S' && currLocation[1] == '?') {
                gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
            } else if (currLocationID >= MIN_MAP_LOCATION && currLocationID <= MAX_MAP_LOCATION) {
                if (idToType(currLocationID) == SEA) {
                    gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                }
            }

            // Check if a double back resulted in a sea move
            switch (currLocationID) {
                case DOUBLE_BACK_1:
                    back = 2;
                    break;
                case DOUBLE_BACK_2:
                    back = 3;
                    break;
                case DOUBLE_BACK_3:
                    back = 4;
                    break;
                case DOUBLE_BACK_4:
                    back = 5;
                    break;
                case DOUBLE_BACK_5:
                    back = 6;
                    break;
                case HIDE:
                    back = 2;
                    break;
                default:
                    back = -1;
                    break;
            }

            // If a double back occured, check to see if it results in landing on a sea
            if (back != -1) {
                if (gameView->path[currPlayer][gameView->upto[currPlayer]-back] == SEA_UNKNOWN) {
                    gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                } else if (gameView->path[currPlayer][gameView->upto[currPlayer]-back] >= MIN_MAP_LOCATION &&
                    gameView->path[currPlayer][gameView->upto[currPlayer]-back] <= MAX_MAP_LOCATION) {
                    if (idToType(gameView->path[currPlayer][gameView->upto[currPlayer]-back]) == SEA) {
                        gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                    }
                }
            }

            // If Dracula is at Castle Dracula (a teleport or known location) he gains 10 health
            if ((currLocation[0] == 'T' && currLocation[1] == 'P') || currLocationID == CASTLE_DRACULA) {
                gameView->health[PLAYER_DRACULA] += LIFE_GAIN_CASTLE_DRACULA;
            }

            // Lose score for vampire maturing
            for (j=5;j<6;j++) {
                if (pastPlays[i+j] == 'V') {
                    gameView->score -= SCORE_LOSS_VAMPIRE_MATURES;
                }
            }

            //Scoring for dracula, decreases by 1 each time his turn ends
            gameView->score -= SCORE_LOSS_DRACULA_TURN;

        } else {

            // Process the other players

            // If they have 0 health, that means they were at the hospital, and now 
            // should have full health
            if (gameView->health[currPlayer] == 0) {
                gameView->health[currPlayer] = GAME_START_HUNTER_LIFE_POINTS;
            }

            // Check for traps, and then Dracula encounters (ignore if at sea, should be included in pastPlays)
            for (j=3;j<7;j++) {
                if (gameView->health[currPlayer] > 0) {
                    if (pastPlays[i+j] == 'T') {
                        gameView->health[currPlayer] -= LIFE_LOSS_TRAP_ENCOUNTER;
                    } else if (pastPlays[i+j] == 'D') {
                        gameView->health[currPlayer] -= LIFE_LOSS_DRACULA_ENCOUNTER;
                        gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_HUNTER_ENCOUNTER;
                    }
                } else {
                    break;
                }
            }

            // If they lose all their health score is lost
            if (gameView->health[currPlayer] < 1) {
                gameView->health[currPlayer] = 0;
                gameView->score -= SCORE_LOSS_HUNTER_HOSPITAL;

                // Manually move them to the hospital


            } else {

                // Otherwise increase health if they rest (cap it at 9)

                // First make sure they'd made more than one move
                if (gameView->upto[currPlayer] > 1) {

                    // If their previous location is the same as their current locaiton,
                    // they've rested, and so increment their health
                    if (gameView->path[currPlayer][(gameView->upto[currPlayer])-2] == currLocationID) {
                        gameView->health[currPlayer] += LIFE_GAIN_REST;
                    }

                    // If their new health is larger than the max health (whatever their starting health is),
                    // then cap it at their original starting health
                    if (gameView->health[currPlayer] > GAME_START_HUNTER_LIFE_POINTS) {
                        gameView->health[currPlayer] = GAME_START_HUNTER_LIFE_POINTS;
                    }
                }
            }
        }

        // Move onto the next player
        currPlayer = (currPlayer+1)%NUM_PLAYERS;
    }

    return gameView;
}


// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    return currentView->round;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    return currentView->currentPlayer;
}

// Get the current score
int getScore(GameView currentView)
{
    return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    return currentView->health[player];
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{

    // If their health is zero, and they're not Dracula, they must be
    // at the hospital
    if (currentView->health[player] == 0 && player != PLAYER_DRACULA) {
        return ST_JOSEPH_AND_ST_MARYS;
    }

    // If unitialised return UNKNOWN_LOCATION
    if (currentView->upto[player] == 0) {
        return UNKNOWN_LOCATION;
    }

    // If location is valid return the location
    if (currentView->path[player][currentView->upto[player]-1] >= MIN_MAP_LOCATION &&
        currentView->path[player][currentView->upto[player]-1] <= MAX_MAP_LOCATION) {
        return currentView->path[player][currentView->upto[player]-1];
    }

    // Otherwise just return it (will return special moves if Drac)
    return currentView->path[player][currentView->upto[player]-1];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{

    // Variable for looping
    int i;

    // Initially fill the trail with -1
    for (i=0;i<TRAIL_SIZE;i++) {
        trail[i] = -1;
    }

    // If the size of the players path is less than the trail size,
    // only loop the size of the path amount of times, otherwise
    // loop TRAIL_SIZE amount of times
    if (currentView->upto[player] < TRAIL_SIZE) {
        for (i=0;i<currentView->upto[player];i++) {
            trail[i] = currentView->path[player][currentView->upto[player]-i-1];
        }
    } else {
        for (i=0;i<TRAIL_SIZE;i++) {
            trail[i] = currentView->path[player][currentView->upto[player]-i-1];
        }
    }
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    return getConnectedLocations(currentView->health[player], numLocations,
                                 from, player, round,
                                road, rail, sea);
}
