#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

void append_space(Buffer * b, int n) {
	b->size += n;
	b->data = realloc(b->data, b->size);
}

void serialize_int(int x, Buffer * b) {
	// htonl :: uint32_t -> uint32_t -- converts the parameter from host byte order to network byte order
	x = htonl(x);

	append_space(b, sizeof(int));

	memcpy( ((char*)b->data) + b->size, &x, sizeof(int));
	b->size += sizeof(int);
}

void serialize_string(char *str, Buffer *b) {
	
	int newlen = strlen(str);

	append_space(b, newlen);

	for (int i=0; i < newlen; ++i) {
		memcpy( ((char*)b->data) + b->size + i, str[i], sizeof(char));	
	}

	b->size += newlen;

}

//serialize_message


