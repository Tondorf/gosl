#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "display.h"

<<<<<<< HEAD

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
void print_current_image(message* msg, int start, int end) {

	move(0,0);
	char *pic = msg->img;
	
	for (int row=0; row < 25; row++) {
		
		char *line = (char*) malloc(81);
		strcpy(line, strtok(pic, '\n'));
		line[81] = '\0';
		printw("%s", line);
		
	}
	
	refresh(); // refresh the screen

}


void callback(const struct message *msg) {
	printf("in callback, tst=%d\n", msg->timestamp);
}

