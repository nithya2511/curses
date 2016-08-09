#ifndef CURSES_HELPERS_H
#define CURSES_HELPERS_H

#include <curses.h>

void logInit();
void logPrint(const char *fmt, ...);
int createWindow(WINDOW **newWin, int y, int x);

#endif /* CURSES_HELPERS_H */
