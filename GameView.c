// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "string.h"


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
    int *health[NUM_PLAYERS];

    // Stores all the moves made in a 2d array (current location stored in corresponding upto)
    int **path[NUM_PLAYERS][GAME_START_SCORE*4];
    int *upto[NUM_PLAYERS];
};


// Adds a trap/vamp to a certain location
void addTrapVamp(LocationID ID, char *num, int **time) {
    time[ID][num[ID]] = 6+1;
    num[ID] += 1;
}

// Removes a trap/vamp from a certain location
void removeTrapVamp(LocationID ID, char *num, int **time) {
    num[ID] -= 1;
    time[ID][num[ID]] = 0;
}

// Reduces the lifespan of traps/vamps by 1 (end of each round)
void reduceTimeTrapVamp(LocationID ID, char *num, int **time, encounter_type t) {

    // If there are no traps/vamps at that location just return
    if (num[ID] == 0) return;

    int i;
    // Store the number of traps/vamps to remove
    int amountToRemove = 0;

    // Loop through starting from most recently added
    for (i=num[ID]-1;i>=0;i++) {

        // Reduce its lifespan by 1
        time[ID][i] -= 1;

        // If its lifespan becomes 0, it gets removed
        if (time[ID][i] == 0) {
            amountToRemove++;
        }
    }

    // If vamp hatchlings are being checked, subtract 13 from score for
    // each one which matures (hits 0)
    if(t == VAMPIRE_HATCHLING){
        gameView->score -= 13*amountToRemove;
    }

    // Remove the number of vamps/traps that need to be removed
    num[ID] -= amountToRemove;
}

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
    //strlen() length, loop through
    
    // Get the size of the pastPlays string
    int pastPlaysSize = strlen(pastplays);

    // The round will be pastPlaysSize/(size of a turn * number of players)
    gameView->round = pastPlaysSize/(SIZE_OF_TURN*NUM_PLAYERS);

    // The current player will be (pastPlaysSize/size of a turn)%number of players
    gameView->currentPlayer = (pastPlaysSize/SIZE_OF_TURN)%NUM_PLAYERS;

    // A variable for looping
    int i;

    // Set the current player (always starts as Lord Godalming)
    int curPlayer = PLAYER_LORD_GODALMING;

    // A array is used to store the number of traps at a certain location
    // A 2d array is used to store the lifetime of each of these traps
    char *trapsNum[MAX_MAP_LOCATION] = {0};
    int **trapsTime[MAX_MAP_LOCATION][3] = {0};

    // A array is used to store the number of vamp hatchlings at a certain location
    // A 2d array is used to store the lifetime of each of these hatchlings
    char *vampsNum[MAX_MAP_LOCATION] = {0};
    int **vampsTime[MAX_MAP_LOCATION][3] = {0};

    // A variable to store the trail, used for processing traps
    int trail[TRAIL_SIZE] = {-1};

    // A variable used to store cities already seen, used for processing traps
    // And an upto variable to keep track of the list (essentially acts as a set)
    int seen[TRAIL_SIZE] = {-1};
    int seenUpto = 0;

    // Set all the upto's to be initially 0 (no moves have been made yet)
    for (i=0;i<5;i++) {
        gameView->upto[i] = 0;
    }

    // Initialise Game score
    gameView->score = GAME_START_SCORE
    ;
    // Initialise hunter's health
    for (i=0;i<NUM_PLAYERS-1;i++) {
        gameView->health[i] = GAME_START_HUNTER_LIFE_POINTS;
    }

    // Initialise Dracula's health
    gameView->health[PLAYER_DRACULA] = GAME_START_BLOOD_POINTS;

    // A for loop is used to process each turn
    for(i=0;i<pastPlaysSize;i+SIZE_OF_TURN){

        // Get the current location by concatenating the second and third character
        char *currLocation = {pastPlays[i+1], pastPlays[i+2]};

        // Move the current location into the struct
        strcpy(gameView->path[currPlayer][gameView->upto[curPlayer]], currLocation);

        // Get the current location id
        LocationID currLocationID = abbrevToID(currLocation);

        // If the current player is Dracula
        if (curPlayer == PLAYER_DRACULA) {

            // If < 3 encounters in current city, an encounter can be added
            if (trapsNum[currLocationID] + vampsNum[currLocationID] < MAX_ENCOUNTERS) {

                // If the current round is divisible by 13 and a vamp, otherwise add a trap
                if ((i/(SIZE_OF_TURN*NUM_PLAYERS))%13 == 0) {
                    addTrapVamp(currLocationID, vampsNum, vampsTime);
                } else {
                    addTrapVamp(currLocationID, vampsNum, vampsTime);
                }
            }

            // For each hunter in the same location as Dracula, Dracula loses 10 health
            for (i=0;i<4;i++) {
                if (gameView->path[i][upto[i]] == currLocation) {
                    gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_HUNTER_ENCOUNTER;
                }
            }

            // If Dracula is at sea he loses 2 health
            if (idToType(currLocationID) == SEA) {
                gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
            }

            // If Dracula is at Castle Dracula he gains 10 health
            if (currLocationID == CASTLE_DRACULA) {
                gameView->health[PLAYER_DRACULA] += LIFE_GAIN_CASTLE_DRACULA;
            }

            //Scoring for dracula, decreases by 1 each time his turn ends
            gameView->score -= SCORE_LOSS_DRACULA_TURN;

        } else {

            // Process the other players

            // Increase health if they rest (cap it at 9)

            // First make sure they'd made more than one move
            if (gameView->upto[currPlayer] > 1) {

                // If their previous location is the same as their current locaiton,
                // they've rested, and so increment their health
                if (gameView->path[currPlayer][(gameView->upto[currPlayer])-1] == currLocation) {
                    gameView->health[currPlayer] += LIFE_GAIN_REST;
                }

                // If their new health is larger than the max health (whatever their starting health is),
                // then cap it at their original starting health
                if (gameView->health[currPlayer] > GAME_START_HUNTER_LIFE_POINTS) {
                    gameView->health[currPlayer] = GAME_START_HUNTER_LIFE_POINTS;
                }
            }

            // Check if a trap is encountered
            if (trapsNum[currLocationID] > 0) {

                // While a trap still exists at that location, subtract health
                while (trapsNum[currLocationID] > 0 || gameView->health[currPlayer] > 0) {
                    removeTrapVamp(currLocationID, trapsNum, trapsTime);
                    gameView->health[currPlayer] -= LIFE_LOSS_TRAP_ENCOUNTER;
                }
            }

            // If their health is < 0 then they can't do anything else, if not then check for
            // an encounter with Dracula
            if (gameView->health[currPlayer] > 0) {

                // If there is an encounter with Dracula, they lose some health
                if (currLocation == gameView->path[PLAYER_DRACULA][gameView->upto[PLAYER_DRACULA]]) {
                    gameView->health[currPlayer] -= LIFE_LOSS_DRACULA_ENCOUNTER;
                }
            } else {
                // Else player is dead, points decrease by 6
                gameView->score -= SCORE_LOSS_HUNTER_HOSPITAL;
            }
        }

        // Remove any traps/vamps which have expired

        // Get Dracula's trail (reduce time of traps and vamps on his trail)
        // Only the cities on his trail will contain traps/vamps, and so only 
        // those need to be checked
        getHistory(gameView, PLAYER_DRACULA, trail);

        // Loop through Draculas trail
        for (i=0;i<TRAIL_SIZE;i++) {

            // If the current location is valid and has not yet been seen
            if (trail[i] >= MAX_MAP_LOCATION && trail[i] <= MAX_MAP_LOCATION &&
                !isIn(seen, seenUpto, trail[i]) && trail[i] != -1) {

                // Add it to the seen array
                seen[seenUpto] = trail[i];

                // Reduce the time for the traps and vamps within that city
                reduceTimeTrapVamp(trail[i], trapsNum, trapsTime, TRAP);
                reduceTimeTrapVamp(trail[i], vampsNum, vampsTime, VAMPIRE_HATCHLING);
            }
        }

        // Move onto the next player
        curPlayer = (curPlayer+1)%NUM_PLAYERS;
    }

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
    //Is this one right?
    return path[currentPlayer][round];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int i;
    //Round greater than 6, full trail
    if(gameView->round>= 6){
        for(i=0;i<6;i++){
            train[i]=path[player][gameView->round];
            gameView->round--;
        }
    }else{// Trail is less than 6
        for(i=0;i<gameView->round;i++){
            trail[i] = path[player][gameView->round];
        }
        //Not sure if filling the rest of the array is right or necessary
        while(i<6){
            trail[i] = -1;
        }

    }

    trail[TRAIL_SIZE] = '\0';
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //Stick lab from 2 weeks ago in here
    return NULL;
}
