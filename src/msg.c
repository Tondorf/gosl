#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h> // htonl
#include "msg.h"

int getBufferSize(struct message *msg) {
	return 3*sizeof(uint32_t) + msg->width * msg->height;
}

void serialize (char *buf, struct message *msg) {
	memcpy(&buf[0], &msg->timestamp, 4);
	memcpy(&buf[4], &msg->width, 4);
	memcpy(&buf[8], &msg->height, 4);
	memcpy(&buf[12], msg->image, msg->width * msg->height);
}

void deserialize (struct message *msg, const char *buf) {
	memcpy(&msg->timestamp, &buf[0], 4);
	memcpy(&msg->width, &buf[4], 4);
	memcpy(&msg->height, &buf[8], 4);
	msg->image = (char*) malloc(msg->width * msg->height);
	memcpy(msg->image, &buf[12], msg->width * msg->height);
}

/* Buffer ist nicht erforderlich

struct Buffer *new_buffer() {
	struct Buffer *b = malloc(sizeof(Buffer));

	b->data = malloc(0);
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
*/


