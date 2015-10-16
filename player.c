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
   char *plays =  "GAM.... SCN.... HZU.... MCD.... DGE.V.. GBU.... SGA.... HMI.... MCD.... DCFT... GLE.... SCD.... HMU.... MCD.... DBOT... GNA.... SCD.... HVI.... MCD.... DTOT... GLE.... SCD.... HVI.... MKL.... DMRT... GNA.... SKL.... HBD.... MBD.... DMIT... GCFT... SBD.... HSZ.... MSZ.... DZUT.V. GMRT... SSZ.... HBD.... MGA.... DMUT... GMR.... SBD.... HKL.... MGA.... DSTT.M. GMIT... SKL.... HBD.... MCD.... DCOT.M. GSTT... SBD.... HSZ.... MKL.... DBUT... GST.... SSZ.... HBD.... MBD.... DLET... GMUT... SBD.... HKL.... MBR.... DPAT.M. GMU.... SKL.... HBD.... MBR.... DGE.V.. GVE.... SBD.... HSZ.... MBR.... DMRT... GBD.... SSZ.... HBD.... MVI.... DGOT.M. GSZ.... SBD.... HKL.... MBD.... DMIT.M. GBD.... SKL.... HBD.... MSZ.... DVET.M. GKL.... SBD.... HSZ.... MBD.... DMUT.M. GBD.... SSZ.... HBD.... MKL.... DSTT.V. GSZ.... SBD.... HKL.... MBD.... DFRT.M. GBD.... SKL.... HBD.... MSZ.... DLIT.M. GKL.... SBD.... HSZ.... MBD.... DNUT.M. GBD.... SSZ.... HBD.... MKL.... DPRT.M. GSZ.... SBD.... HKL.... MBD.... DVIT.M. GBD.... SKL.... HBD.... MSZ.... DMUT.M. GKL.... SBD.... HSZ.... MKL.... DMI.VM. GCD.... SKL.... HKL.... MCD.... DVET.M. GKL.... SCD.... HCD.... MKL.... DFLT.M. GCD.... SKL.... HKL.... MCD.... DGOT.M. GKL.... SCD.... HCD.... MKL.... DMRT.M. GCD.... SKL.... HKL.... MCD.... DTOT.M. GKL.... SCD.... HCD.... MKL.... DCFT.V. GCD.... SKL.... HKL.... MCD.... DNAT.M. GKL.... SCD.... HCD.... MKL.... DLET.M. GCD.... SKL.... HKL.... MCD.... DBUT.M. GKL.... SCD.... HCD.... MKL.... DPAT.M. GCD.... SKL.... HKL.... MCD.... DGET.M. GKL.... SCD.... HCD.... MKL.... DSTT.M. GCD.... SKL.... HKL.... MCD.... DCO.VM. GKL.... SCD.... HCD.... MKL.... DHAT.M. GCD.... SKL.... HKL.... MCD.... DBRT.M. GKL.... SCD.... HCD.... MKL.... DLIT.M. GCD.... SKL.... HKL.... MCD.... DFRT.M. GKL.... SCD.... HCD.... MKL.... DNUT.M. GCD.... SKL.... HKL.... MCD.... DPRT.V. GGA.... SBE.... HBE.... MGA.... DVIT.M. GKL.... SBC.... HBC.... MSZ.... DBDT.M. GBE.... SSZ.... HSO.... MBE.... DKLT.M. GBC.... SBC.... HBC.... MBC.... DSZT.M. GSO.... SBE.... HBE.... MSZTD.. DBET.M. GBC.... SBC.... HBC.... MSZ.... DBCT.M. GBET... SSZ.... HSO.... MBE.... DCN.VM. GBCT... SBC.... HBC.... MBC.... DGAT.M. GBC.... SBE.... HBE.... MSZ.... DCDT.M. GBE.... SBC.... HBC.... MBC.... DKLT... GBC.... SSZ.... HSO.... MBE.... DBDT... GSZ.... SBC.... HBC.... MBC.... DSZT... GBC.... SBE.... HBE.... MSZTD.. DBET.V. GBETD.. SBC.... HBC.... MJM.... DBCT.M. GBE.... SSZ.... HSO.... MJM.... DCNT.M. GBE.... SBCT... HBC.... MJM.... DGAT.M. GKL.... SBE.... HBE.... MJM.... DCDT.M. GKL.... SBE.... HBE.... MSZ.... DKLT... GKLTD.. SBE.... HJM.... MPR.... DSZT... GKL.... SJM.... HSZTD.. MPR.... DZA.V.. GKL.... SSZ.... HSZ.... MPR.... DVIT.M. GKL.... SSZ.... HSZ.... MVE.... DPRT.M. GKL.... SSZ.... HGA.... MBD.... DNUT.M. GKL.... SGA.... HBC.... MVIT... DSTT... GKL.... SBC.... HBC.... MVI.... DZUT... GKL.... SBC.... HBC.... MBD.... DMUT.V. GKL.... SBC.... HCN.... MVI.... DZAT... GKL.... SCN.... HBC.... MPRT... DVIT... GKL.... SBC.... HBC.... MPR.... DPRT.M. GKL.... SBC.... HBC.... MVE.... DNUT.M. GKL.... SBC.... HCN.... MBD.... DSTT.M. GKL.... SBE.... HSZ.... MSZ.... DFRT.M. GGA.... SSA.... HKL.... MJM.... DCO.VM. GSZ.... SIO.... HGA.... MSJ.... DHAT.M. GKL.... STS.... HCD.... MJM.... DBRT.M. GGA.... SNP.... HCD.... MSJ.... DLIT.M. GCD.... SBI.... HCD.... MJM.... DNUT.M. GCD.... SAS.... HCD.... MSJ.... DSTT.M. GCD.... SVE.... HCD.... MJM.... DGET.V. GCD.... SVE.... HCD.... MSJ.... DPAT.M. GCD.... SVE.... HCD.... MJM.... DLET.M. GCD.... SMI.... HCD.... MSJ.... DBUT.M. GCD.... SGO.... HCD.... MJM.... DAMT.M. GCD.... SFL.... HCD.... MSJ.... DCOT.M. GCD.... SBI.... HCD.... MSJ.... DSTT.M. GCD.... SBI.... HCD.... MSJ.... DGE.VM. GCD.... SBI.... HCD.... MSJ.... DZUT.M. GCD.... SBI.... HCD.... MSJ.... DMUT.M. GCD.... SNP.... HCD.... MSJ.... DNUT.M. GCD.... SNP.... HCD.... MSJ.... DPRT.M. GCD.... SNP.... HCD.... MSJ.... DVIT.M. GCD.... SBI.... HCD.... MSJ.... DZAT.V. GCD.... SNP.... HCD.... MSJ.... DSZT.M. GCD.... SRO.... HCD.... MSJ.... DKLT.M. GCD.... SNP.... HCD.... MSJ.... DBDT.M. GCD.... SRO.... HCD.... MSJ.... DHIT.M. GCD.... SRO.... HCD.... MSJ.... DD2T.M. GCD.... SNP.... HCD.... MSJ....";
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
