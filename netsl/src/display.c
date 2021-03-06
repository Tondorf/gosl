#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "msg.h"
#include "display.h"

void setup_display() {
	initscr();   // ncurses initialization
	curs_set(0); // invisible cursor
}

void cleanup_display() {
	endwin(); // clean ncurses shutdown
}

/*
	image[row][col] >>> image[row*width+col];

	uint32_t width;		// normally 80
	uint32_t height;	// normally 25, may vary
	char *image;		// dimension is width x height
*/
static void print_current_image(const struct message* msg, int start, int end) {
	end = 80; // end is ignored and 80 is used, should be handled properly later

	//move(0,0); // start in the upper left corner
	char *pic = msg->image; // get a ptr to the actual image
	
	for (int row=0; row < 25; row++) { // iterate over rows
		char *original_line = pic + row * msg->width; // pointer to the start of the line to print
		char *line = (char*) malloc(end-start+1); // allocate a line because we modify it
		strcpy(line, original_line + start); // get a line from the right start
		line[end-start] = '\0'; // terminate it

		mvprintw(row, 0, "%s", line); // print it
		free(line); // free it
	}
	
	refresh(); // refresh the screen
}


void prntscreen(const struct message *msg, const struct prog_info *pinfo);

void callback(const struct message *msg, const struct prog_info *pinfo) {
	//printf("in callback, tst=%d\n", msg->timestamp);
	
	// calculate the actual offset to use
	//int start = msg->timestamp % msg->width  +  pinfo->client_offset;
	//print_current_image(msg, start, start+80);
	
	prntscreen(msg, pinfo);
}


//int init = 0;
//int rows;
//int cols;
void prntscreen(const struct message *msg, const struct prog_info *pinfo) {
	static int init = 0;
	static int rows;
	static int cols;
	static char *img = NULL;
	static int w;
	static int h;
	static int f;
	if (!img && msg->image) {
		img = msg->image;
		w = msg->width;
		h = msg->height;
		f = msg->frames;
	}
	if (!img) {
		printf("awaiting state ... %d\r", msg->timestamp);
		return;
	}
	
	if (!init) {
//		printf("init start\n");
		initscr();
		curs_set(0); // invisible cursor
		getmaxyx(stdscr, rows, cols);

		init = 1;
//		printf("init end\n");
	}

	int frame = msg->timestamp;
	int right = pinfo->client_offset;
	int left = right + cols;

//	for (int y=0; y<h; y++)
//		for (int x=0; x<w; x++)
//			mvaddch(y,x,img[y*w+x]);


//	printf("loop\n");
	int rowoffset = (rows-h)/2;
	//int coloffset = left-frame;
	int imgoff = (frame % f)  * w * h;
	for (int y=0; y<h; y++) { // y<msg->height; y++) {
   		for (int x=left-frame; x<cols; x++) {
			if (x<0)
				continue;
			//mvaddch(y + rowoffset, x, ('0' + x-(left-frame)+y));
			int p = x-(left-frame);
			mvaddch(y + rowoffset, x, p>=w-1?' ':img[imgoff+(y*w+p)]);
		}
	}

	refresh();
}
