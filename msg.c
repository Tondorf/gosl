#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "msg.h"

#if INTERFACE

#define INITIAL_SIZE 32

struct Buffer {
	int size;
	void *data;
};

struct message {
	long timestamp;
	int width;		// varies
	int height;		// normally 80
	char **image;	// dimension is width x height
};


#endif

struct Buffer *new_buffer() {
	struct Buffer *b = malloc(sizeof(Buffer));

	b->data = malloc(INITIAL_SIZE);
	b->size = 0;

	return b;
}

void append_space(Buffer * b, int bytes) {
	b->size += bytes;
	b->data = realloc(b->data, b->size);
}

void serialize_int(int x, Buffer * b) {
	// htonl :: uint32_t -> uint32_t
	x = htonl(x);

	append_space(b, sizeof(int));

	memcpy(((char *)b->data) + b->next, &x, sizeof(int));
	b->next += sizeof(int);
}

//serialize_string
//serialize_message


