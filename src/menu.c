#include <stddef.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <curses.h>
#include "menu.h"
#include "list.h"

list_t *gMenuList = NULL;

struct menuItem_s {
	void *next;
	void *subList;
	char name[24];
	union {
		uint16_t flags;
		struct {
			uint16_t hasSubMenu :1;
			uint16_t isActive :1;
			uint16_t isEnabled :1;
		};
	};
	int (*handler)(int);
};

struct menuItem_s *makeMenu(const char *name) {
	struct menuItem_s *new = NULL;
	new = malloc(sizeof(struct menuItem_s));
	if (new == NULL) {
		printf("Malloc failed!\n");
		return NULL;
	}
	strncpy(new->name, name, 23);
	new->isActive = 0;
	new->hasSubMenu = 0;
	new->subList = NULL;
	return new;
}

struct menuItem_s *findMenuItem(const char *name) {
	// find menu list in gMenuList;
	if (gMenuList == NULL)
		return NULL;
	struct menuItem_s *this = (struct menuItem_s *) gMenuList;
	while (this != NULL) {
		if (strcmp(this->name, name) == 0)
			return this;
		this = this->next;
	}
	return NULL;
}

int registerMenuItem(const char *name, int (*handler)(int)) {
	struct menuItem_s *item;
	item = makeMenu(name);
	if (item == NULL)
		return -1;
	item->handler = handler;
	return listAdd(&gMenuList, item);

}

int registerSubMenu(const char *pName, const char *name, int (*handler)(int)) {
	struct menuItem_s *item;
	item = findMenuItem(pName);
	if (item == NULL)
		return -1;
	struct menuItem_s *sub;
	sub = makeMenu(name);
	if (sub == NULL)
		return -1;
	sub->handler = handler;
	if (item->hasSubMenu == 0) {
		item->subList = NULL;
		item->hasSubMenu = 1;
	}
	return listAdd((list_t **)&(item->subList), sub);
}

int mainMenuPos=0;
int subMenuPos=0;

void updateSubMenu(int dir)
{
	if (mainMenuPos == 0 || dir == 0)
		return;
	subMenuPos += dir;
	if (subMenuPos <= 0)
		subMenuPos = 1;
}

void updateMainMenu(int dir)
{
	if (mainMenuPos == 0 || dir == 0)
		return;
	mainMenuPos += dir;
	if (mainMenuPos <= 0)
		mainMenuPos = 1;
	subMenuPos = 0;
}

void menuNavigate(int key) {
	int mainMenuDir=0, subMenuDir=0;
	switch(key) {
	case 'b':
		mainMenuPos = 1;
		subMenuPos = 0;
		break;
	case 'w':
	case KEY_UP:
		subMenuDir = -1;
		break;
	case 's':
	case KEY_DOWN:
		subMenuDir = 1;
		break;
	case 'a':
	case KEY_LEFT:
		mainMenuDir = -1;
		break;
	case 'd':
	case KEY_RIGHT:
		mainMenuDir = 1;
		break;
	default:
		mainMenuPos = 0;
		subMenuPos = 0;
	}
	updateMainMenu(mainMenuDir);
	updateSubMenu(subMenuDir);
	printf("(mainMenuPos, subMenuPos) %d %d\n", mainMenuPos, subMenuPos);
}

void drawMenu(WINDOW *menuWin)
{

}

void printMenuGraph()
{
	char printChar;
	int curMainPos=1, curSubPos = 0;
	struct menuItem_s *sub, *item;
	item = (struct menuItem_s *)gMenuList;
	while (item != NULL) {
		printChar = (curMainPos == mainMenuPos) ? '*' : ' ';
		printf("%d: %s%c ", curMainPos, item->name, printChar);
		if ((printChar == '*') && item->hasSubMenu) {
			sub = (struct menuItem_s *)item->subList;
			curSubPos = 1;
			while (sub != NULL) {
				printChar = (curSubPos == subMenuPos) ? '*' : ' ';
				printf("-> %s%c ", sub->name, printChar);
				sub = sub->next;
				curSubPos++;
			}
		}
		printf("\n");
		item = item->next;
		curMainPos++;
	}
}
