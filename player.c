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
   char *plays =  "GMN.... SMA.... HST.... MSZ.... DGE.V.. GMN.... SBA.... HBU.... MBD.... DCFT... GMN.... SLS.... HBU.... MVI.... DNAT... GMN.... SMA.... HBU.... MZA.... DPAT... GMN.... SCA.... HBO.... MMU.... DSTT... GMN.... SLS.... HLE.... MZU.... DFRT... GMN.... SAO.... HNAT... MMR.... DNUT.V. GMN.... SBB.... HBO.... MMS.... DPRT.M. GMN.... SNA.... HMR.... MAO.... DBRT... GMN.... SBO.... HBU.... MBB.... DHAT.M. GMN.... SLE.... HBU.... MNA.... DD2T.M. GMN.... SNA.... HBU.... MBO.... DLIT.M. GMN.... SBO.... HBO.... MLE.... DNUT.M. GMN.... SMR.... HLE.... MNA.... DPR.VM. GMN.... SBU.... HNA.... MBO.... DBRT.M. GMN.... SBU.... HBO.... MMR.... DHAT.M. GMN.... SBU.... HMR.... MBU.... DCOT.M. GMN.... SBO.... HBU.... MBU.... DLIT.M. GMN.... SLE.... HBU.... MBU.... DFRT.M. GMN.... SNA.... HBU.... MBO.... DNUT.V. GMN.... SBO.... HBO.... MLE.... DSTT.M. GMN.... SMR.... HLE.... MNA.... DMUT.M. GMN.... SBU.... HNA.... MBO.... DVIT.M. GMN.... SBU.... HBO.... MMR.... DBDT.M. GMN.... SBU.... HMR.... MBU.... DKLT.M. GMN.... SBO.... HBU.... MBU.... DCDT.M. GMN.... SLE.... HBU.... MBU.... DGA.VM. GMN.... SNA.... HBU.... MBO.... DBCT.M. GMN.... SBO.... HBO.... MLE.... DBET.M. GMN.... SMR.... HLE.... MNA.... DSJT.M. GMN.... SBU.... HNA.... MBO.... DSOT.M. GMN.... SBU.... HBO.... MMR.... DSAT.M. GMN.... SBU.... HMR.... MBU.... DVAT.V. GMN.... SBO.... HBU.... MBU.... DATT.M. GMN.... SLE.... HBU.... MBU.... DD2T.M. GMN.... SNA.... HBU.... MBO.... DSJT.M. GMN.... SBO.... HBO.... MLE.... DBET.M. GMN.... SMR.... HLE.... MNA.... DSOT.M. GMN.... SBU.... HNA.... MBO.... DVAT.M. GMN.... SBU.... HBO.... MMR.... DAT.VM. GMN.... SBU.... HMR.... MBU.... DD2T.M. GMN.... SBO.... HBU.... MBU.... DSAT.M. GMN.... SLE.... HBU.... MBU.... DIO..M. GMN.... SNA.... HBU.... MBO.... DAS..M. GMN.... SBO.... HBO.... MLE.... DBIT.M. GMN.... SMR.... HLE.... MNA.... DNPT.V. GMN.... SBU.... HNA.... MBO.... DROT.M. GMN.... SBU.... HBO.... MMR.... DFLT.M. GMN.... SBU.... HMR.... MBU.... DGOT... GMN.... SBO.... HBU.... MBU.... DMRT... GMN.... SLE.... HBU.... MBU.... DMIT.M. GMN.... SNA.... HBU.... MBO.... DZUT.M. GMN.... SBO.... HBO.... MLE.... DST.VM. GMN.... SMRT... HLE.... MNA.... DFRT.M. GMN.... SBU.... HNA.... MBO.... DNUT.M. GMN.... SBU.... HBO.... MMR.... DPRT... GMN.... SBU.... HMR.... MBU.... DBRT.M. GMN.... SBO.... HBU.... MBU.... DHAT.M. GMN.... SLE.... HBU.... MBU.... DD2T.V. GMN.... SNA.... HBU.... MBO.... DLIT.M. GMN.... SBO.... HBO.... MLE.... DNUT.M. GMN.... SMR.... HLE.... MNA.... DPRT.M. GMN.... SBU.... HNA.... MBO.... DBRT.M. GMN.... SBU.... HBO.... MMR.... DHAT.M. GMN.... SBU.... HMR.... MBU.... DCOT.M. GMN.... SBO.... HBU.... MBU.... DLI.VM. GMN.... SLE.... HBU.... MBU.... DFRT.M. GMN.... SNA.... HBU.... MBO.... DNUT.M. GMN.... SBO.... HBO.... MLE.... DSTT.M. GMN.... SMR.... HLE.... MNA.... DZUT.M. GMN.... SBU.... HNA.... MBO.... DMUT.M. GMN.... SBU.... HBO.... MMR.... DZAT.V. GMN.... SBU.... HMR.... MBU.... DVIT.M. GMN.... SBO.... HBU.... MBU.... DPRT.M. GMN.... SLE.... HBU.... MBU.... DBRT.M. GMN.... SNA.... HBU.... MBO.... DLIT.M. GMN.... SBO.... HBO.... MLE.... DNUT.M. GMN.... SMR.... HLE.... MNA.... DMUT.M. GMN.... SBU.... HNA.... MBO.... DZU.VM. GMN.... SBU.... HBO.... MMR.... DSTT.M. GMN.... SBU.... HMR.... MBU.... DGET.M. GMN.... SBO.... HBU.... MBU.... DCFT.M. GMN.... SLE.... HBU.... MBU.... DTOT.M. GMN.... SNA.... HBU.... MBO.... DBAT.M. GMN.... SBO.... HBO.... MLE.... DD2T.V. GMN.... SMR.... HLE.... MNA.... DSRT.M. GMN.... SBU.... HNA.... MBO.... DALT.M. GMN.... SBU.... HBO.... MMR.... DMS..M. GMN.... SBU.... HMR.... MBU.... DCGT.M. GMN.... SBO.... HBU.... MBU.... DTS..M. GMN.... SLE.... HBU.... MBU.... DROT.M. GMN.... SNA.... HBU.... MBO.... DNP.VM. GMN.... SBO.... HBO.... MLE.... DBIT.M. GMN.... SMR.... HLE.... MNA.... DD2T... GMN.... SBU.... HNA.... MBO.... DHIT.M. GMN.... SBU.... HBO.... MMR....";
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
