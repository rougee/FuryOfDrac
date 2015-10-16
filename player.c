/**
 * Runs a player's game turn ...
 *
 * Can produce either a Hunter player or a Dracula player
 * depending on the setting of the I_AM_DRACULA #define
 *
 * This is a dummy version of the real player.c used when
 * you submit your AIs. It is provided so that you can
 * test whether your code is likely to compile ...
 *
 * Note that it used to drive both Hunter and Dracula AIs.
 * It first creates an appropriate view, and then invokes the
 * relevant decide-my-move function, which should use the 
 * registerBestPlay() function to send the move back.
 *
 * The real player.c applies a timeout, so that it will halt
 * decide-my-move after a fixed amount of time, if it doesn't
 * finish first. The last move registered will be the one
 * used by the game engine. This version of player.c won't stop
 * your decide-my-move function if it goes into an infinite
 * loop. Sort that out before you submit.
 *
 * Based on the program by David Collien, written in 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#ifdef I_AM_DRACULA
#include "DracView.h"
#include "dracula.h"
#else
#include "HunterView.h"
#include "hunter.h"
#endif

// moves given by registerBestPlay are this long (including terminator)
#define MOVE_SIZE 3

// The minimum static globals I can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

int main(int argc, char *argv[])
{
#ifdef I_AM_DRACULA
   DracView gameState;
   //char *plays = "GZA.... SED.... HZU.... MZU....";
   char *plays =  "GAM.... SCN.... HZU.... MCD.... DGE.V.. GCO.... SVR.... HMU.... MGA.... DPAT... GAM.... SCN.... HNU.... MCN.... DLET... GCO.... SVR.... HPR.... MVR.... DNAT... GHA.... SSO.... HLI.... MCN.... DCFT... GLI.... SBE.... HHA.... MVR.... DBOT... GHA.... SBC.... HLI.... MSO.... DTOT.V. GLI.... SKL.... HHA.... MBE.... DBAT.M. GHA.... SCD.... HLI.... MBC.... DSRT.M. GLI.... SGA.... HHA.... MKL.... DALT.M. GHA.... SBD.... HLI.... MCD.... DMAT.M. GLI.... SVI.... HHA.... MGA.... DGRT.M. GHA.... SPR.... HLI.... MBD.... DCAT.M. GLI.... SLI.... HHA.... MVI.... DLS.VM. GHA.... SHA.... HLI.... MPR.... DSNT.M. GLI.... SLI.... HHA.... MLI.... DSRT.M. GHA.... SHA.... HLI.... MHA.... DBAT.M. GLI.... SLI.... HHA.... MLI.... DTOT.M. GHA.... SHA.... HLI.... MHA.... DBOT.M. GLI.... SLI.... HHA.... MLI.... DNAT.V. GHA.... SHA.... HLI.... MHA.... DCFT.M. GLI.... SBR.... HCO.... MCO.... DMRT.M. GST.... SVI.... HAM.... MAM.... DZUT.M. GFR.... SMU.... HAM.... MAM.... DSTT.M. GLI.... SMU.... HAM.... MAM.... DBUT.M. GCO.... SMI.... HAM.... MAM.... DPAT.M. GCO.... SMRT... HAM.... MAM.... DLE.VM. GAM.... SMR.... HAM.... MAM.... DNAT... GAM.... SCF.... HAM.... MAM.... DCFT.M. GAM.... SBO.... HAM.... MAM.... DTOT.M. GAM.... SBO.... HAM.... MAM.... DSRT.M. GAM.... SNAT... HAM.... MAM.... DBAT.M. GAM.... SCFT... HAM.... MAM.... DMS..V. GAM.... SBO.... HAM.... MAM.... DCGT... GAM.... SBO.... HAM.... MAM.... DTS.... GAM.... SNA.... HAM.... MAM.... DNPT.M. GAM.... SCF.... HAM.... MAM.... DROT.M. GAM.... SBO.... HAM.... MAM.... DFLT.M. GAM.... SBO.... HAM.... MAM.... DVET... GAM.... SNA.... HAM.... MAM.... DMI.VM. GAM.... SCF.... HAM.... MAM.... DZUT... GAM.... SBO.... HAM.... MAM.... DSTT.M. GAM.... SBO.... HAM.... MAM.... DBUT.M. GAM.... SNA.... HAM.... MAM.... DPAT.M. GAM.... SNA.... HAM.... MAM.... DNAT.M. GAM.... SNATD.. HAM.... MAM.... DLET.V. GAM.... SNA.... HAM.... MAM.... DEC..M. GAM.... SNA.... HAM.... MAM.... DPLT.M. GAM.... SNA.... HAM.... MAM.... DLOT.M. GAM.... SNA.... HAM.... MAM.... DSWT.M. GAM.... SNA.... HAM.... MAM.... DLVT... GAM.... SNA.... HAM.... MAM.... DMNT.M. GAM.... SNA.... HAM.... MAM.... DED.V.. GAM.... SNA.... HAM.... MAM.... DNS..M. GAM.... SNA.... HAM.... MAM.... DHAT.M. GAM.... SNA.... HAM.... MAM.... DLIT.M. GAM.... SNA.... HAM.... MAM.... DNUT.M. GAM.... SNA.... HAM.... MAM.... DSTT.M. GAM.... SNA.... HAM.... MAM.... DGET.V. GAM.... SNA.... HAM.... MAM.... DZUT... GAM.... SNA.... HAM.... MAM.... DMUT.M. GAM.... SNA.... HAM.... MAM.... DZAT.M. GAM.... SNA.... HAM.... MAM.... DVIT.M. GAM.... SNA.... HAM.... MAM.... DPRT.M. GCO.... SCF.... HCO.... MCO.... DNUT.M. GST.... STO.... HZUT... MST.... DLI.V.. GLIVD.. SBO.... HMR.... MZU.... DCOT.M. GBU.... SNA.... HMS.... MST.... DHAT.M. GCOT... SCF.... HTS.... MBU.... DBRT.M. GCO.... STO.... HRO.... MCO....";
   PlayerMessage msgs[3] = { "", "", "" };
   gameState = newDracView(plays,msgs);
   decideDraculaMove(gameState);
   disposeDracView(gameState);
#else
   HunterView gameState;
   char *plays = "GAM.... SCN.... HZU.... MCD.... DC?.V..";
   //char *plays =  "GAM.... SAM.... HAM.... MAM.... DGE.V.. GNS.... SCO.... HBU.... MAM....";
   PlayerMessage msgs[3] = { "", "", "" };
   gameState = newHunterView(plays,msgs);
   decideHunterMove(gameState);
   disposeHunterView(gameState);
#endif 
   printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
   return EXIT_SUCCESS;
}

// Saves characters from play (and appends a terminator)
// and saves characters from message (and appends a terminator)
void registerBestPlay (char *play, PlayerMessage message) {
   strncpy(latestPlay, play, MOVE_SIZE-1);
   latestPlay[MOVE_SIZE-1] = '\0';
     
   strncpy(latestMessage, message, MESSAGE_SIZE);
   latestMessage[MESSAGE_SIZE-1] = '\0';
}
