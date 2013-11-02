#ifndef __IMAGE
#define __IMAGE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINELEN 20000

char *readImage(const char* filename, int *cols, int *rows);

#endif
