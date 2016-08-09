#include "cursesHelpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int createWindow(WINDOW **newWin, int y, int x)
{
	*newWin = newwin(50, 50, 0, 0);
	if (*newWin == NULL)
		return -1;
	return 0;
}

FILE *logFile;

void logInit()
{
	unlink("out.log");
	printf("Got to log init\n");
	logFile = fopen("out.log", "w");
	if(logFile == NULL) {
		printf("Unable to create logFile\n");
		exit(-1);
	}
}

void logPrint(const char *fmt, ...)
{
	int len=0;
	char logBuf[128];
	va_list args;

	va_start(args, fmt);
	len = vsnprintf(logBuf, 128, fmt, args);
	va_end(args);
	if (len >= 128) {
		logBuf[128-1] = 0;
		len = 128-1;
	}
	//printf("log: %s", logBuf);
	//fwrite(logBuf, 1, len, logFile);
	fprintf(logFile, logBuf);
	fflush(logFile);
}
