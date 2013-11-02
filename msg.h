/* This file was automatically generated.  Do not edit! */
typedef struct message message;
typedef struct Buffer Buffer;
void serialize_message(struct message *msg,Buffer *b);
void serialize_string(char *str,Buffer *b);
void serialize_int(int x,Buffer *b);
void append_space(Buffer *b,int n);
struct Buffer *new_buffer();
struct message {
	long timestamp;
	int width;		// varies
	int height;		// normally 80
	char **image;	// dimension is width x height
};
struct Buffer {
	int size;
	void *data;
};
#define INITIAL_SIZE 32
#define INTERFACE 0
