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
			*cols = len>*cols?len:*cols;
			(*rows)++;
		}
		
		(*cols)--;
		ret = (char *)malloc(*cols * *rows + 1);
		memset(ret, ' ', *cols * *rows);
		fseek(f, 0, SEEK_SET);
		for (int r=0; r<*rows; r++) {
			fgets(&ret[r * *cols], LINELEN, f);
		}
			
		fclose(f);
	}
	free(linebuf);

	printf("image size: %d x %d\n", *cols, *rows);

	 for (int y=0; y < *rows; y++) {
	 	for (int x=0; x < *cols; x++) {
			//mvaddch(y,x,img[y * *cols +x]);
			printf("%c", ret[y * *cols +x]);
		}
		printf("\n");
	}

	return ret;
}

