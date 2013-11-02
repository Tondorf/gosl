#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h> // htonl
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
	//x = htonl(x);

	append_space(b, sizeof(int));

	memcpy( ((char*)b->data) + b->size, &x, sizeof(int));
	b->size += sizeof(int);
}

void serialize_string(char *str, Buffer *b) {
	
	int newlen = strlen(str);

	append_space(b, newlen);

	int i = 0;
	while (i < newlen) {
		memcpy( ((char*)b->data) + b->size + i, str, sizeof(char));	
		str++;
		i++;
	}

	b->size += newlen;

}

void serialize_message(struct message *msg, Buffer *b) {
	serialize_int(msg->timestamp, b); // does this also work for long?
	serialize_int(msg->width, b);
	serialize_int(msg->height, b);
	for (int i=0; i < msg->width; i++)
		serialize_string(msg->image[i], b);
}


int main() {

	Buffer *buf = new_buffer();
	//serialize_string("lol", buf);
	serialize_int(42, buf);
	printf("buf: size:%d data:%p\n", buf->size, buf->data);

}


