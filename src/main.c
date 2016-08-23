#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "cursesHelpers.h"
#include "menu.h"
#include "mainWinManager.h"
#include "handler.h"

void initMenuEntries() {
	logPrint("entered menu entries\n");
	registerMenuItem("File", NULL);
	registerSubMenu("File", "New", NULL);
	registerSubMenu("File", "Open", NULL);
	registerSubMenu("File", "Quit", exitProgram);
	registerMenuItem("Edit", NULL);
	registerSubMenu("Edit", "Undo", NULL);
	registerSubMenu("Edit", "Redo", NULL);
	registerMenuItem("View", NULL);
	registerSubMenu("View", "Format", NULL);
	registerSubMenu("View", "Print", NULL);
	registerMenuItem("Help", NULL);
	registerSubMenu("Help", "About", NULL);
	registerSubMenu("Help", "Author", authorProg);
}

int main() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);


	logInit();
	initMenuEntries();
	initMainWindow();

	while (1) {
		if (updateMainWin() < 0)
			break;
	}
	destroyMainWin();
	endwin();
	return 0;
}
