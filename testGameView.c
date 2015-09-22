// testGameView.c ... test the GameView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "GameView.h"

int main()
{
    int i;
    GameView gv;

    printf("Test basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    gv = newGameView("", messages1);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 0);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed default tests\n");
    disposeGameView(gv);
    //Extra tests for Dracula trail
    printf("Test for Dracula trail 2 rounds");
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DD1....",messages2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 2);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == STRASBOURG);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DRACULA) == SEA_UNKNOWN);
    print("passed test 2 rounds\n")
    disposeGameView(gv);
    printf("Test for players not exceeding max life points");
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GGE.... SGE.... HGE.... MGE....", messages 2);
    assert(getCurrentPlayer(gv) == PLAYER_DRACULA);
    assert(getRound(gv) == 2);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == GENEVA);
    assert(getLocation(gv, PLAYER_DR_SEWARD)== GENEVA);
    assert(getHealth(gv, PLAYER_LORD_GODALMING == GAME_START_HUNTER_LIFE_POINTS));
    assert(getHealth(gv, PLAYER_DR_SEWARD)== GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING)== GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA)== GAME_START_BLOOD_POINTS)
    printf("passed extra tests\n");

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(getLocation(gv,PLAYER_DRACULA) == GENEVA);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 5);
    assert(getHealth(gv,PLAYER_DRACULA) == 30);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula restoring blood points in his castle(exceeding the maximum)\n");
    gv = newGameView("GLO.... SPA.... HGE.... MMI.... DCD.... "
                     "GST.... SST.... HST.... MST.... DCD....",messages3);
    assert(getLocation(gv, PLAYER_DRACU) == CASTLE_DRACULA);
    assert(getHealth(gv,PLAYER_DRACULA)==50);
    assert(getLocation(gv, PLAYER_LORD_GODALMING == STRASBOURG);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == STRASBOURG);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == STRASBOURG);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == STRASBOURG);
    getHistory(gv, PLAYER_LORD_GODALMING,history);
    assert(history[0] == LONDON);
    assert(history[1] == STRASBOURG);
    assert(history[4] == UNKNOWN_LOCATION);
    getHistory(gv, PLAYER_DRACULA,history);
    assert(history[0] == CASTLE_DRACULA);
    assert(history[1] == CASTLE_DRACULA);
    disposeGameView(gv);
    printf("passed\n");

    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Drac View)\n");
    PlayerMessage messages5[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DEC.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages5);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == ENGLISH_CHANNEL);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("", messages1);
    printf("Checking Galatz road connections\n");
    edges = connectedLocations(gv,&size,GALATZ,PLAYER_LORD_GODALMING,0,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    printf("Checking Ionian Sea sea connections\n");
    edges = connectedLocations(gv,&size,IONIAN_SEA,PLAYER_LORD_GODALMING,0,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    printf("Checking Athens rail connections (none)\n");
    edges = connectedLocations(gv,&size,ATHENS,PLAYER_LORD_GODALMING,0,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    printf("passed\n");
    disposeGameView(gv);
    //Extra tests for connections
    gv = newGameView("", messages1);
    printf("Checking Venice ANY connections\n");
    edges = connectedLocations(gv,&size,VENICE,PLAYER_LORD_GODALMING,0,1,1,1);
    for (i=0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[VENICE]); assert(seen[ADRIATIC_SEA]);
    assert(seen[FLORENCE]);assert(seen[MILAN]); assert(seen[GENOA]);
    assert(seen[VIENNA]); assert(seen[MUNICH]);
    free(edges);
    printf("Checking Atlantic Ocean conections\n");
    edges = connectedLocations(gv,&size, ATLANTIC_OCEAN,PLAYER_LORD_GODALMING,0,0,0,1);
    assert (size == 6); assert(seen[ATLANTIC_OCEAN]); assert(seen[NORTH_SEA]);
    assert(seen[IRISH_SEA]); assert(seen[ENGLISH_CHANNEL]); assert(seen[BAY_OF_BISCAY]);
    assert(seen[MEDITERRANEAN_SEA]);
    free(edges);
    printf("Checking Vienna sea connection(none)\n");
    edges = connectedLocations(gv, &size, VIENNA,PLAYER_LORD_GODALMING,0,0,0,1);
    assert (size == 1);
    assert(edges[0] == VIENNA);
    free(edges);
    printf("Checking Black sea road connection(none)\n");
    edges = connectedLocations(gv, &size, BLACK_SEA,PLAYER_LORD_GODALMING,0,1,0,0);
    assert(size == 1);
    assert(edges[0] == BLACK_SEA);
    free(edges);
    printf("Checking Galway rail connection(none)\n");
    edges = connectedLocations(gv, &size, GALWAY,PLAYER_LORD_GODALMING,0,0,1,0);
    assert(size == 1);
    assert(edges[0] == GALWAY);
    free(edges);
    disposeGameView(gv);
    printf("ALL TESTS PASSED\n");
    return 0;



}

