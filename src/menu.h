#ifndef SRC_MENU_H_
#define SRC_MENU_H_

int resetMenuPos();
void printMenuGraph();
void menuNavigate(int key);
int registerSubMenu(const char *pName, const char *name, int (*handler)(int));
int registerMenuItem(const char *name, int (*handler)(int));
void displayMenu(WINDOW *win);
int moveMenuHorizontal(int dir);
//int moveMenu(struct menuItem_s *start, int dir);
int moveMenuVertical(int dir);


#endif /* SRC_MENU_H_ */
