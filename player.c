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
   char *plays =  "GAM.... SCN.... HZU.... MCD.... DGE.V.. GNS.... SSZ.... HGEVD.. MGA.... DCFT... GAM.... SSA.... HCFTD.. MBC.... DNAT... GNS.... SIO.... HJM.... MSO.... DBOT... GAM.... SAT.... HBE.... MBD.... DSRT... GNS.... SVA.... HKL.... MKL.... DALT... GAM.... SAT.... HBC.... MBC.... DGRT... GNS.... SVA.... HBE.... MSO.... DMAT... GAM.... SAT.... HSA.... MBD.... DCAT.M. GNS.... SVA.... HBE.... MKL.... DLST.M. GAM.... SAT.... HKL.... MBC.... DSNT.M. GNS.... SVA.... HBC.... MSO.... DSRT.M. GAM.... SAT.... HSO.... MVR.... DALT.M. GBU.... SIO.... HVR.... MCN.... DGR.VM. GST.... SSA.... HCN.... MBC.... DCAT.M. GNU.... SSO.... HBC.... MGA.... DLST.M. GST.... SSJ.... HGA.... MCD.... DMAT.M. GZU.... SZA.... HCD.... MKL.... DSNT.M. GST.... SJM.... HKL.... MCD.... DSRT.M. GNU.... SBE.... HCD.... MKL.... DALT.V. GST.... SSO.... HKL.... MCD.... DGRT.M. GZU.... SSJ.... HCD.... MKL.... DCAT.M. GST.... SZA.... HKL.... MCD.... DLST.M. GNU.... SJM.... HCD.... MKL.... DMAT.M. GST.... SBE.... HKL.... MCD.... DSNT.M. GZU.... SSA.... HCD.... MKL.... DSRT.M. GST.... SSO.... HKL.... MCD.... DBA.VM. GNU.... SSJ.... HCD.... MKL.... DTOT.M. GST.... SZA.... HKL.... MCD.... DBOT.M. GZU.... SJM.... HCD.... MKL.... DNAT.M. GST.... SBE.... HKL.... MCD.... DD2T.M. GNU.... SSO.... HCD.... MKL.... DCFT.M. GST.... SSJ.... HKL.... MCD.... DMRT.V. GPA.... SVA.... HBE.... MGA.... DGOT.M. GBOTT.. SSO.... HBC.... MCN.... DMIT... GSN.... SBE.... HSZ.... MVR.... DZUT.M. GSR.... SBC.... HGA.... MSZ.... DSTT... GAL.... SCN.... HBC.... MJM.... DGET.M. GGR.... SBC.... HKL.... MSZ.... DCFT.M. GGR.... SKL.... HBE.... MGA.... DNA.VM. GGR.... SBE.... HJM.... MBC.... DBOT.M. GGR.... SJM.... HSZ.... MKL.... DTOT.M. GGR.... SSZ.... HJM.... MBE.... DMRT.M. GGR.... SJM.... HSZ.... MJM.... DZUT.M. GGR.... SSZ.... HGA.... MSZ.... DGET.M. GGR.... SGA.... HBC.... MJM.... DCFT.V. GGR.... SBC.... HKL.... MSZ.... DBOT.M. GGR.... SKL.... HBE.... MGA.... DTOT.M. GGR.... SBE.... HJM.... MBC.... DMRT.M. GMA.... SSZ.... HZA.... MGA.... DGOT.M. GLS.... SBC.... HJM.... MKL.... DFLT.M. GMA.... SGA.... HZA.... MCD.... DROT.M. GLS.... SKL.... HJM.... MCD.... DBI.VM. GAO.... SCD.... HZA.... MCD.... DNPT.M. GLS.... SCD.... HJM.... MCD.... DD2T.M. GMA.... SCD.... HZA.... MCD.... DAS..M. GLS.... SCD.... HJM.... MCD.... DIO..M. GAO.... SCD.... HZA.... MCD.... DATT.M. GLS.... SCD.... HJM.... MCD.... DVAT.V. GMA.... SCD.... HZA.... MCD.... DSAT.M. GLS.... SCD.... HJM.... MCD.... DSOT.M. GAO.... SCD.... HZA.... MCD.... DVRT... GMS.... SGA.... HVI.... MGA.... DD2T... GTS.... SCN.... HVE.... MSZ.... DSJT.M. GRO.... SVRT... HMU.... MJM.... DVAT.M. GNP.... SBE.... HMI.... MSZ.... DAT.VM. GRO.... SSA.... HGO.... MZA.... DIO..M. GNP.... SIO.... HFL.... MVI.... DBS.... GTS.... SVAT... HVE.... MVE.... DCNT.M. GRO.... SATV... HPR.... MFL.... DVRT.M. GMI.... SVA.... HVI.... MRO.... DSOT... GGO.... SAT.... HVE.... MMI.... DSJT... GFL.... SVA.... HMU.... MGO.... DBET... GRO.... SAT.... HMI.... MFL.... DBCT... GMI.... SVA.... HGO.... MRO.... DCNT.M. GGO.... SAT.... HFL.... MMI.... DGAT.M. GFL.... SIO.... HFL.... MMR.... DCDT.M. GVE.... SAS.... HZU.... MMR.... DKLT.M. GAS.... SAS.... HMR.... MMS.... DSZ.VM. GAS.... SAS.... HMS.... MAL.... DZAT.M. GAS.... SAS.... HAL.... MMS.... DSJT.M. GAS.... SAS.... HMS.... MAL.... DSOT.M. GAS.... SAS.... HAL.... MGR.... DBCT.M. GAS.... SAS.... HGR.... MGR.... DKLT.M. GAS.... SBI.... HAL.... MAL.... DCDT.V. GBI.... SNP.... HAL.... MMS.... DGAT.M. GNP.... SBI.... HMS.... MAO.... DCNT.M. GBI.... SNP.... HAO.... MAO.... DVRT.M. GNP.... SBI.... HAO.... MAO.... DSOT.M. GBI.... SNP.... HAO.... MAO.... DSJT.M. GNP.... SBI.... HAO.... MAO.... DZAT.M. GBI.... SNP.... HAO.... MAO.... DBD.VM. GNP.... SBI.... HAO.... MAO.... DVIT.M. GBI.... SNP.... HAO.... MAO.... DMUT.M. GNP.... SBI.... HAO.... MAO.... DZUT.M. GBI.... SNP.... HAO.... MAO.... DSTT.M. GNP.... SBI.... HAO.... MAO.... DFRT.M. GBI.... SNP.... HAO.... MAO.... DNUT.V. GAS.... SBI.... HAO.... MAO.... DLIT.M. GVE.... SAS.... HAO.... MAO.... DBRT.M. GMI.... SVE.... HAO.... MAO.... DHAT.M. GZU.... SVE.... HAO.... MAO.... DCOT.M. GZU.... SBD.... HAO.... MAO.... DBUT.M. GLIT... SBD.... HAO.... MAO.... DPAT.M. GFR.... SBD.... HAO.... MAO.... DCF.V.. GFR.... SBD.... HAO.... MAO.... DTOT.M. GST.... SBR.... HAO.... MAO.... DSRT.M. GCOT... SPR.... HAO.... MAO.... DBOT... GCO.... SVI.... HAO.... MAO.... DNAT.M. GCO.... SZA.... HAO.... MAO.... DPAT.M. GST.... SZA.... HAO.... MAO.... DCFT.V. GCO.... SZA.... HEC.... MEC.... DTOT.M. GST.... SVI.... HLO.... MLO.... DBAT.M. GFR.... SSZ.... HEC.... MSW.... DSRT.M. GBU.... SPR.... HLO.... MLV.... DSNT.M. GBO.... SBR.... HSW.... MMN.... DLST.M. GSRT... SHA.... HLO.... MLV.... DCAT.M. GTOT... SCO.... HEC.... MMN.... DGR.V.. GCF.... SST.... HLO.... MED.... DALT.M. GTO.... SFR.... HSW.... MED.... DMAT... GMR.... SFR.... HSW.... MMN.... DSNT.M. GPA.... SBR.... HLO.... MLV.... DLST.M. GST.... SLI.... HLO.... MSW.... DCAT.M. GFL.... SHA.... HLO.... MSW.... DGRT.V. GRO.... SHA.... HLV.... MLO.... DALT.M. GRO.... SLI.... HSW.... MLO.... DSRT.M. GNP.... SHA.... HLV.... MMN.... DBAT.M. GBI.... SBR.... HMN.... MSW.... DTOT.M. GNP.... SHA.... HSW.... MLV.... DMRT.M. GBI.... SCO.... HLV.... MMN.... DGOT.M. GNP.... SBO.... HMN.... MED.... DVE.VM. GBI.... SSRT... HED.... MLV.... DMIT... GNP.... SBO.... HLV.... MIR.... DMUT.M. GBI.... SMA.... HIR.... MAO.... DNUT.M. GNP.... SPA.... HAO.... MNS.... DPRT.M. GTS.... SGE.... HNS.... MHA.... DVIT.M. GIO.... SMR.... HHA.... MNS.... DZAT.V. GBS.... SCF.... HNS.... MHA.... DSJT.M. GCN.... SMR.... HHA.... MBR.... DD4T.M. GBE.... SCF.... HBR.... MVIT... DATT.M. GSO.... SNA.... HHA.... MVE.... DD2T.M. GSJT... SBO.... HPR.... MFL.... DD1.... GBE.... SMR.... HVI.... MVE.... DHI..M. GVR.... SMS.... HVE.... MFL.... DCN.V.. GBS.... SCG.... HFL.... MVE.... DBCT.M. GVR.... SMS.... HVE.... MFL.... DKLT.M. GBE.... SCG.... HFL.... MVE.... DCDT.M. GVR.... SMS.... HFL.... MVI.... DD5T... GBS.... STS.... HVE.... MHA.... DHIT... GVR.... SRO.... HAS.... MHA.... DCNT.V. GCNTD.. SZU.... HVE.... MHA.... DD2T.M. GJM.... SMI.... HAS.... MCO....";
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
