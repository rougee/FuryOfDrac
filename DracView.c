// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"

#include "string.h"

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
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    //Store traps and vamps+gameview
    int round;
    int currentPlayer;
    int score;
    ////////////////////////////////////////////
    ///////Structs
    //Individual arrays for each of size 5;(0,1,2,3 = hunters)4=drac
    int health[NUM_PLAYERS];
    int location[NUM_PLAYERS];
    //traps and vampires
    int locationTraps[NUM_MAP_LOCATIONS];
    int locationVamp[NUM_MAP_LOCATIONS];
    //need to store trail
    LocationID dracTrail[TRAIL_SIZE];

    
    
    GameView game; // realised this makes several functions below easier
};



// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{

    DracView dracView = malloc(sizeof(struct dracView));
 
    
    //set basics from gameview: round, current player, score, health, location and dracula's trail
    GameView gv;
    gv = newGameView(pastPlays, messages);
    dracView->game = gv;
    dracView->round = getRound(gv);
    dracView->currentPlayer = getCurrentPlayer(gv);
    dracView->score = getScore(gv);
    getHistory(gv,PLAYER_DRACULA,dracView->dracTrail);
    
    
    int i,j,a;
    for (a = 0; a < NUM_PLAYERS; a++ ) {
        dracView->health[a] = getHealth(gv,a);
        dracView->location[a] = getLocation(gv,a);
    }

    i = 0;
    
    
    //loop to store locations of all traps and vamps
    int pastPlaysSize = strlen(pastPlays);
    
    //if trap expires/ vamp hatches
    
    if(dracView->currentPlayer == PLAYER_DRACULA){
        if( pastPlays[i+EXPIRY_INDEX] == 'M'){
            dracView->locationTraps[dracView->dracTrail[FINAL_TRAIL_IND]]--;
        }
        else if (pastPlays[i+EXPIRY_INDEX] == 'V'){
            dracView->locationVamp[dracView->dracTrail[FINAL_TRAIL_IND]]--;
        }
    }
    
    //TRAPS AND VAMPS ARRAYS
    
    for(i=0;i<pastPlaysSize;i=i+SIZE_OF_TURN){ //standard loop to traverse previous turns

        char currLocation[2];
        currLocation[0] = pastPlays[i+1];
        currLocation[1] = pastPlays[i+2];

        LocationID currLocationID = abbrevToID(currLocation); //convert 2character location to LocationID

        if(dracView->currentPlayer == PLAYER_DRACULA) {
            if (dracView->locationTraps[currLocationID] + dracView->locationVamp[currLocationID] < MAX_ENCOUNTERS){
                if(pastPlays[i+TRAP_IND] == 'T') { //trap
                    dracView->locationTraps[currLocationID]++;
                }
                if(pastPlays[i+VAMP_IND] == 'V') { //vamp
                    dracView->locationVamp[currLocationID]++;
                }
            }
        }
        else { //hunter encounters trap or vamp, disables, repeatedly up to 3 per city, traps first
            //alternatively check the pastPlays for V or D
            for(j=0; j < MAX_ENCOUNTERS; j++){
                if (dracView->locationTraps[currLocationID] > 0) {
                    dracView->locationTraps[currLocationID]--;
                }
                else if (dracView->locationVamp[currLocationID] > 0) {
                    dracView->locationVamp[currLocationID]--;
                }
            }
        }
    }
    return dracView;
}


// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
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
    getHistory(currentView->game, player, trail); //storing the gameView also makes this easier

    *start = trail[1];
    *end = trail[0];
    
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                int *numTraps, int *numVamps)
{
    *numTraps = currentView->locationTraps[where];
    *numVamps = currentView->locationVamp[where];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->game, player, trail); //storing the gameView makes this easier
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
    
    LocationID *where = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
    int i, num;

    where[0] = currentView->location[PLAYER_DRACULA];

    num = 1;
    
    //first case, first move
    
    if(getRound(currentView->game)== 0) {
        //move anywhere except hospital
        for(i=0; i<NUM_MAP_LOCATIONS; i++) {
            if(i != ST_JOSEPH_AND_ST_MARYS) {
                where[i] = i;
            }
        }
        num = NUM_MAP_LOCATIONS -1;
    }
    
    else {
        //first find whether dobule backed or not, hid or not
        int hideState = FALSE;
        int doubleState = FALSE;
        //LocationID *dracTrail;
        getHistory(currentView->game, PLAYER_DRACULA, currentView->dracTrail); // Is that allowed???
        int j;
        for (j=0; j<TRAIL_SIZE; j++){
            if(currentView->dracTrail[j] == HIDE){
                hideState = TRUE; //hidden
            }
            if( (currentView->dracTrail[j] <= DOUBLE_BACK_5) && (currentView->dracTrail[j] >= DOUBLE_BACK_1)){
                doubleState = TRUE;  //double backed
                //int doubleNum = dracTrail[j] - DOUBLE_BACK_CONVERSION; //converts to 1-5
            }
        }
        
        //add all adjacent except those on  trail
        //first get adjacents
        int tempNum;
        LocationID *adjacent = connectedLocations(currentView->game, &tempNum,
                                                  whereIs(currentView, PLAYER_DRACULA),
                                                  PLAYER_DRACULA,
                                                  getRound(currentView->game), road, FALSE, sea);
        //copy to the adjacent array elements that are not on the trail
        int whereCounter;
        int k;
        for(whereCounter=0; whereCounter < tempNum; whereCounter++) { //for each adjacent
            int isInTrail = FALSE;
            for (k=0; k<TRAIL_SIZE; k++) { //check if its in trail
                if (currentView->dracTrail[k] == adjacent[whereCounter]) {
                    isInTrail = TRUE;
                }
            }
            if(isInTrail == FALSE) {
                where[num] = adjacent[whereCounter];
                num++; //only increases if not in trail
            }
        }
        //if doubleBack not true, add all locations on trail except current

        if (doubleState == FALSE){
            int l;
            for(l = 1; l<TRAIL_SIZE; l++) { //skip ind 0
                where[num] = currentView->dracTrail[l];
                num++;
            }
        
        }
        
        //if hide not true, add current location unless currently at sea
        if ((hideState == FALSE) && (idToType(currentView->dracTrail[0] != SEA)) ){
            where[num] = currentView->dracTrail[0];
            num++;
        }
    }
    
    *numLocations = num;
    return where;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    LocationID *where = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
    int i = 0;
    
    if(player == PLAYER_DRACULA) { //dracula uses the dracula moves
        where = whereCanIgo(currentView, numLocations, road, sea);
    }
    
    else { //hunter
        // if(getRound(currentView->game)== 0) {//hunter first round, freedom to move anywhere
        //     for(i = 0; i < NUM_MAP_LOCATIONS; i++) {
        //         where[i] = i;
        //     }
        //     //*numLocations = NUM_MAP_LOCATIONS;
        // }
        // else { //else hunter moves adjacent, or multiple by rail, handled by connectedLocations
            where = connectedLocations(currentView->game, &i,
                                       whereIs(currentView, player),player,
                                       currentView->round, road, rail, sea);
        //}
    }

    *numLocations = i;
    return where;
}