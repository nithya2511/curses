#ifndef SRC_MENU_H_
#define SRC_MENU_H_

void printMenuGraph();
void menuNavigate(int key);
int registerSubMenu(const char *pName, const char *name, int (*handler)(int));
int registerMenuItem(const char *name, int (*handler)(int));

#endif /* SRC_MENU_H_ */
