#include "apilib.h"
#include <stdio.h>
#include <string.h>

void HariMain(void)
{
	// Init
	int fileHandler;
	char sth[] = {'R', 'e', 'p', 'l', 'a', 'c', 'e', 'd'};
	char fileName[10];

	// fileName
	strcpy(fileName, "make.bat");

	// call
	api_fopen(fileName);
	api_sprintf(fileName, sth);
	api_end();

}

