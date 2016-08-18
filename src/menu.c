#include <stddef.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <curses.h>
#include "menu.h"
#include "list.h"
#include "cursesHelpers.h"

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
	return listAdd((list_t **) &(item->subList), sub);
}

void resetList(struct menuItem_s * item) {

	while (item != NULL) {
		item->isActive = 0;
		item->isEnabled = 0;
		if (item->hasSubMenu)
			resetList(item->subList);
		item = item->next;
	}
}

int resetMenuPos() {
	if (gMenuList == NULL)
		return -1;
	resetList((struct menuItem_s *) gMenuList);
	return 0;
}

int moveMenu(struct menuItem_s *start, int dir) {
	struct menuItem_s *item, *prev = NULL, *toMark = NULL;
	item = start;
	while (item != NULL) {
		if (item->isActive || item->isEnabled)
			break;
		prev = item;
		item = item->next;
	}
	if (item == NULL) {
		// This is only for the first run when nothing is active.
		start->isActive = 1;
		return 1;
	}

	if (dir < 0) {
		if (prev == NULL)
			return 0;
		toMark = prev;
	} else {
		if (item->next == NULL)
			return 0;
		toMark = item->next;
	}
	// un-mark the current item.
	item->isActive = 0;
	item->isEnabled = 0;

	toMark->isActive = 1;
	return 1;
}

int moveMenuHorizontal(int dir) {
	if (gMenuList == NULL)
		return -1; // list is empty.
	struct menuItem_s *item, *tmp = NULL;
	item = (struct menuItem_s *) gMenuList;
	while (item != NULL) {
		if (item->isActive || item->isEnabled)
			break;
		item = item->next;
	}
	tmp = item; // backup
	if (moveMenu((struct menuItem_s *) gMenuList, dir)) {
		if (tmp && tmp->hasSubMenu)
			resetList(tmp->subList);
		return 1;
	}
	return 0;
}

int moveMenuVertical(int dir) {
	if (gMenuList == NULL)
		return -1; // list is empty.

	struct menuItem_s *item, *sub = NULL;
	item = (struct menuItem_s *) gMenuList;
	while (item != NULL) {
		if (item->isActive || item->isEnabled)
			break;
		item = item->next;
	}
	if (item == NULL) {
		// the first key to be pressed was a down arrow.
		// call move vertical once to put the cursor at
		// first menu option.
		return moveMenuHorizontal(1);
	}

	if (item->hasSubMenu) {
		item->isEnabled = 1;
		sub = item->subList;
		if (moveMenu(sub, dir)) {
			item->isActive = 0;
		}
	}
	return 0;
}
void menuNavigate(int key) {

	switch (key) {
	case 'b':
		// TODO reset menuItem_s list
		break;
	case 'w':
	case KEY_UP:
		moveMenuVertical(-1);
		break;
	case 's':
	case KEY_DOWN:
		moveMenuVertical(1);
		break;
	case 'a':
	case KEY_LEFT:
		moveMenuHorizontal(-1);
		break;
	case 'd':
	case KEY_RIGHT:
		moveMenuHorizontal(1);
		break;
	default:
		logPrint("Got to dfl %d\n", key);
	}
}

#include <stdlib.h>
void printMenuGraph() {
	struct menuItem_s *sub, *item;
	item = (struct menuItem_s *) gMenuList;
	 system("clear");
	while (item != NULL) {
		if (item->isActive)
			printf("[");
		printf("%s", item->name);
		if (item->isActive)
			printf("]");
		if (item->isEnabled && item->subList) {
			sub = (struct menuItem_s *) item->subList;
			while (sub != NULL) {
				printf(" -> ");
				if (sub->isActive)
					printf("[");
				printf("%s", sub->name);
				if (sub->isActive)
					printf("]");
				sub = sub->next;
			}
		}
		printf("\r\n");
		item = item->next;
	}
	printf("\r\n");
}

#define MENU_ITEM_WIDTH 5
void printItem(WINDOW *win, int row, int col, struct menuItem_s *item)
{
	if (item->isActive){
		wattron(win,A_REVERSE);
		mvwprintw(win, row, col*MENU_ITEM_WIDTH, "%s", item->name);
		wattroff(win,A_REVERSE);
	} else {
		mvwprintw(win, row, col*MENU_ITEM_WIDTH, "%s", item->name);
	}
}

void displayMenu(WINDOW *win) {
	werase(win);
	int i=0, j;
	struct menuItem_s *sub, *item;
	item = (struct menuItem_s *) gMenuList;
	while (item != NULL) {
		j = 0;
		printItem(win, j, i, item);
		if (item->isEnabled && item->subList) {
			sub = (struct menuItem_s *) item->subList;
			while (sub != NULL) {
				j++;
				printItem(win, j, i, sub);
				sub = sub->next;
			}
		}
		item = item->next;
		i++;
	}
	wrefresh(win);
}



