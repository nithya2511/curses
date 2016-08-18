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
	MWS_IDLE, MWS_MENU_NAV, MWS_PRINT,
};

WINDOW *mainWin = NULL;
int mainWinState = MWS_IDLE;

void initMainWindow() {
	createWindow(&mainWin, 50, 50);
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
	case MWS_IDLE:
		ch = wgetch(mainWin);
		if (ch == 'q' || ch == 'Q')
			return -1; // exit the main loop.
		if (ch == 'b' || ch == 'B') {
			mainWinState = MWS_MENU_NAV;
			resetMenuPos();
		}
		break;
	case MWS_MENU_NAV:
		ch = wgetch(mainWin);
		if (ch == 10)
			return 0;

		menuNavigate(ch);
		//printMenuGraph();
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
