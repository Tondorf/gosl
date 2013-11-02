#include "image.h"

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
