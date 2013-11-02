/* This file was automatically generated.  Do not edit! */
typedef struct Buffer Buffer;
void append_space(Buffer *b,int n);
struct Buffer *new_buffer();
typedef struct message message;
void deserialize(struct message *msg,const char *buf);
void serialize(char *buf,struct message *msg);
int getBufferSize(struct message *msg);
struct message {
	uint32_t timestamp;
	uint32_t width;		// varies
	uint32_t height;		// normally 80
	//char **image;	// dimension is width x height
	char *image;	// dimension is width x height
};
struct Buffer {
	int size;
	void *data;
};
#define INITIAL_SIZE 32
#define INTERFACE 0
