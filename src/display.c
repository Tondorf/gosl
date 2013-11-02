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
void print_current_image(struct message* msg, int start, int end) {
	end++; // suppress unused warning :->
	// end is ignored and 80 is used, should be considered later

	move(0,0); // start in the upper left corner
	char *pic = msg->image; // get a ptr to the actual image
	
	for (int row=0; row < 25; row++) { // iterate over rows
		char *original_line;
		original_line = strtok(pic, "\n");

		char *line = (char*) malloc(81); // allocate a line because we modify it
		strcpy(line, original_line + start); // get a line from the right start
		line[80] = '\0'; // terminate it
		printw("%s", line); // print it
		free(line); // free it
	}
	
	refresh(); // refresh the screen

}


void callback(const struct message *msg) {
	printf("in callback, tst=%d\n", msg->timestamp);
}

