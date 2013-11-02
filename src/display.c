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

void callback(const struct message *msg, const struct prog_info *pinfo) {
	printf("in callback, tst=%d\n", msg->timestamp);
	
	// calculate the actual offset to use
	int start = msg->timestamp % msg->width  +  pinfo->client_offset;

	print_current_image(msg, start, start+80);
	
}



void prntscreen(const struct message *msg, const struct prog_info *pinfo) {
	static int init = 0;
	static int rows;
	static int cols;
	if (!init) {
		initscr();
		getmaxyx(stdscr, rows, cols);

		init = 1;
	}

	int frame = msg->frame;
	int right = pinfo->client_offset;
	int left = right + cols;

	int pos = frame + off;


	refresh();
}
