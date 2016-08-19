/*
 * mainWinManager.c
 *
 *  Created on: 12-Aug-2016
 *      Author: nithyav
 */

#include <stdlib.h>
#include "cursesHelpers.h"
#include "menu.h"

enum {
	MSW_INIT, MWS_IDLE, MWS_MENU_NAV, MWS_PRINT,
};
int show = 0;
WINDOW *mainWin = NULL;
int mainWinState = MSW_INIT;

void initMainWindow() {
	createWindow(&mainWin, 100, 100, 0, 0);
	if (mainWin == NULL) {
		logPrint("Failed to create main window\n");
		exit(-1);
	}
	werase(mainWin);
	keypad(mainWin, TRUE);
	resetMenuPos();
	displayMenu(mainWin);
}

int updateMainWin() {
	int ch = 0;
	switch (mainWinState) {
	case MSW_INIT:
		resetMenuPos();
		displayMenu(mainWin);
		mainWinState = MWS_IDLE;
		break;
	case MWS_IDLE:
		logPrint("Waiting for command to begin or quit\n");
		ch = wgetch(mainWin);
		if (ch == 'b' || ch == 'B') {
			moveMenuHorizontal(1);// highlight the first menu item in the first run.
			displayMenu(mainWin);
			mainWinState = MWS_MENU_NAV;
		}
		break;
	case MWS_MENU_NAV:
		ch = wgetch(mainWin);
		menuNavigate(ch);
		displayMenu(mainWin);
		//printMenuGraph();
		break;
	case MWS_PRINT:
		break;
	}
	return 0;
}

void destroyMainWin() {
	delwin(mainWin);
}
