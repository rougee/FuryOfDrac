// testGameView.c ... test the GameView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "GameView.h"


int isEqual(LocationID *a, int asize, LocationID *b, int bsize) {
    int i, j, in;

    for (i=0;i<asize;i++) {
        in = 0;

        for (j=0;j<bsize;j++) {
            if (a[i] == b[j]) {
                in = 1;
            }
        }

        if (in == 0) {
            return 0;
        }
    }

    return 1;
}

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
    printf("passed\n");
    disposeGameView(gv);

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




    printf("Test for Dracula doubling back x2 back at sea, and losing blood points (unknown locations)\n");
    PlayerMessage messages6[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DC?.... "
                     "GGE.... SGE.... HGE.... MGE.... DD2....", messages6);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_2);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_2);
    assert(history[1] == CITY_UNKNOWN);
    assert(history[2] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back x3 back at sea, and losing blood points (unknown locations)\n");
    PlayerMessage messages7[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DC?.... "
                     "GST.... SST.... HST.... MST.... DS?.... "
                     "GGE.... SGE.... HGE.... MGE.... DD3....", messages7);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_3);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_3);
    assert(history[1] == SEA_UNKNOWN);
    assert(history[2] == CITY_UNKNOWN);
    assert(history[3] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 6);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);


    printf("Test for Dracula doubling back x4 back at sea, and losing blood points (unknown locations)\n");
    PlayerMessage messages8[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DC?.... "
                     "GST.... SST.... HST.... MST.... DS?.... "
                     "GST.... SST.... HST.... MST.... DC?.... "
                     "GGE.... SGE.... HGE.... MGE.... DD4....", messages8);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_4);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_4);
    assert(history[1] == CITY_UNKNOWN);
    assert(history[2] == SEA_UNKNOWN);
    assert(history[3] == CITY_UNKNOWN);
    assert(history[4] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 6);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);


    printf("Test for Dracula doubling back x5 back at sea, and losing blood points (unknown locations) "
            "and capping of hunter health\n");
    PlayerMessage messages9[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DC?.... "
                     "GST.... SST.... HST.... MST.... DS?.... "
                     "GST.... SST.... HST.... MST.... DC?.... "
                     "GST.... SST.... HST.... MST.... DS?.... "
                     "GGE.... SGE.... HGE.... MGE.... DD5....", messages9);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_5);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_5);
    assert(history[1] == SEA_UNKNOWN);
    assert(history[2] == CITY_UNKNOWN);
    assert(history[3] == SEA_UNKNOWN);
    assert(history[4] == CITY_UNKNOWN);
    assert(history[5] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 8);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);


    PlayerMessage genericMessage[] = {"Some generic message"};

    
    printf("Test for single Dracula encounter for 2 players + single trap encounter "
           "for the first player and two trap encounters for the second player\n");
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DC?.... "
                     "GPATD.. SPAD... HCFT... MCF.... DC?.... ", genericMessage);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 20);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - 6);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - 4);
    assert(getHealth(gv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS - 2);
    assert(getHealth(gv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);



    printf("Test for score after two rounds and two vamps hatching\n");
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DC?V.V. "
                     "GPA.... SPA.... HCF.... MCF.... DC?..V. ", genericMessage);
    assert(getScore(gv) == GAME_START_SCORE - 2 - 26);
    printf("passed\n");
    disposeGameView(gv);



    printf("Test 0 health for the first player and updated score\n");
    gv = newGameView("GGEDTT. SGE.... HGE.... MGE.... DC?.... "
                     "GPAT... SPA.... HCF.... MCF.... DC?.... ", genericMessage);
    printf("SCORE IS %d\n", getScore(gv));
    assert(getScore(gv) == GAME_START_SCORE - 2 - 6);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 0);
    printf("passed\n");
    disposeGameView(gv);



    printf("Test restored health for the first player (after resting at hospital)\n");
    gv = newGameView("GGEDTT. SGE.... HGE.... MGE.... DC?.... "
                     "GPAT... SPA.... HCF.... MCF.... DC?.... "
                     "GJM.... SPA.... HCF.... MCF.... DC?.... ", genericMessage);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    printf("passed\n");
    disposeGameView(gv);


    printf("Test connected locations\n");
    gv = newGameView("GGE.... SSZ.... HGE.... MGE.... DC?.... "
                     "GPA.... SSZ.... HCF.... MCF.... DC?.... "
                     "GPA.... SSZ.... HCF.... MCF.... DC?.... ", genericMessage);
    int as;
    LocationID *a;
    a = connectedLocations(gv,&as,SZEGED,PLAYER_LORD_GODALMING,3,1,1,1);
    LocationID b[] = {SZEGED,ST_JOSEPH_AND_ST_MARYS,ZAGREB,BUDAPEST,KLAUSENBURG,BUCHAREST,BELGRADE,
                      VIENNA,GALATZ,CONSTANTA,SOFIA,
                      VARNA,SALONICA,PRAGUE,VENICE}; 
    assert(as == 15);
    assert(isEqual(a,as,b,15) == 1);

    printf("passed\n");
    disposeGameView(gv);

    
    return 0;
}

