/* This file was automatically generated.  Do not edit! */
typedef struct Buffer Buffer;
void serialize_int(int x,Buffer *b);
void append_space(Buffer *b,int bytes);
struct Buffer *new_buffer();
typedef struct message message;
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
