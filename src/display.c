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
	// end is ignored and 80 is used, should be considered later

	move(0,0); // start in the upper left corner
	char *pic = msg->image; // get a ptr to the actual image
	
	for (int row=0; row < 25; row++) { // iterate rows
		char *line = (char*) malloc(81);
		strcpy(line, strtok(pic, '\n')); // get a line
		line[80] = '\0'; // terminate
		printw("%s", line+start);
	}
	
	refresh(); // refresh the screen

}


void callback(const struct message *msg) {
	printf("in callback, tst=%d\n", msg->timestamp);
}

