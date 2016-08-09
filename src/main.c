#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "cursesHelpers.h"
#include "menu.h"

void initMenuEntries()
{
	registerMenuItem("File", NULL);
	registerSubMenu ("File", "New", NULL);
	registerSubMenu ("File", "Open", NULL);
	registerSubMenu ("File", "Quit", NULL);
	registerMenuItem("Edit", NULL);
	registerSubMenu ("Edit", "Undo", NULL);
	registerSubMenu ("Edit", "Redo", NULL);
	registerMenuItem("View", NULL);
	registerSubMenu ("View", "Format", NULL);
	registerSubMenu ("View", "Print", NULL);
	registerMenuItem("Help", NULL);
	registerSubMenu ("Help", "About", NULL);
	registerSubMenu ("Help", "Author", NULL);
}

int main()
{
	initMenuEntries();
	while (1) {
		int ch = getchar(); // wait here again and again.
		if (ch == 'q' || ch == 'Q')
			break;
		if (ch == 10)
			continue;
		menuNavigate(ch);
		printMenuGraph();
	}
	return 0;
}
