#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h> // MAX

#include "image.h"

#define LINELEN 20000

char *readImage(const char* filename, int *cols, int *rows) {
	
	printf("fopen\n");
	FILE *f = fopen(filename, "r");
	printf("ok\n");
	
	*cols = 0;
	*rows = 0;
	char *linebuf = (char *)malloc(LINELEN);
	char *ret = NULL;
	if (f) {
		while (fgets(linebuf, LINELEN, f)) {
			int len = strlen(linebuf);
			*cols = MAX(len, *cols);
			(*rows)++;
		}
		
		cols--;
		ret = (char *)malloc(*cols * *rows);
		memset(ret, ' ', *cols * *rows);
		fseek(f, 0, SEEK_SET);
		for (int r=0; r<*rows; r++) {
			fgets(&ret[r * *cols], *cols, f);
		}
			
		fclose(f);
	}
	free(linebuf);

	return ret;
}

