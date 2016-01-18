#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h> // MAX

#include "image.h"

#define LINELEN 20000

char *readImage(const char* filename, int *cols, int *rows, int *imgz) {
	
	printf("fopen\n");
	FILE *f = fopen(filename, "r");
	printf("ok\n");
	
	*cols = 0;
	*rows = 0;
	int maxrows = 0;
	*imgz = 1;
	char *linebuf = (char *)malloc(LINELEN);
	char *ret = NULL;
	if (f) {
		while (fgets(linebuf, LINELEN, f)) {
			int len = strlen(linebuf);
			if ( len > 2 && linebuf[0] == '.' && linebuf[1] == '.') {
				(*imgz)++;
				(*rows)=0;
			}
			*cols = len>*cols?len:*cols;
			(*rows)++;
			maxrows = (*rows>maxrows)?*rows:maxrows;
		}
		(*rows)=maxrows;
		
		(*cols)--;
		ret = (char *)malloc(*imgz * *cols * *rows + 1);
		memset(ret, ' ', *imgz * *cols * *rows);
		fseek(f, 0, SEEK_SET);
		
		for (int fp=0; fp<(*imgz); fp++) {
			for (int r=0; r<*rows; r++) {
				int base = (fp * *rows * *cols) + r * *cols;
				fgets(&ret[base], LINELEN, f);
				if (ret[base] == '.' && ret[base+1] == '.') {
					ret[base] = ' ';
					ret[base+1] = ' ';
					break;
				}
			}
		}
		for (int i=0; i < (*imgz * *cols * *rows); i++)
			if (ret[i] == '\n' || ret[i] == '\r')
				ret[i] = ' ';
		
		fclose(f);
	}
	free(linebuf);

	printf("image size: %d x %d\n%d frames\n", *cols, *rows, *imgz);

	for (int y=0; y < *rows; y++) {
	 	for (int x=0; x < *cols; x++) {
			//mvaddch(y,x,img[y * *cols +x]);
			printf("%c", ret[y * *cols +x]);
		}
		printf("\n");
	}

	return ret;
}


