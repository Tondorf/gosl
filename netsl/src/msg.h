#ifndef __MSG
#define __MSG

#include <stdint.h>

//typedef struct Buffer {
//	int size;
//	void *data;
//} Buffer;

struct message {
	uint32_t timestamp;
	uint32_t width;		// normally 80
	uint32_t height;	// normally 25, may vary
	uint32_t frames; // number of frames
	//char **image;	// dimension is width x height
	char *image;	// dimension is width x height
/*
 * image[row][col] >>> image[row*width+col];
 */

};

int getBufferSize(struct message *msg);
void serialize(char *buf,struct message *msg);
void deserialize(struct message *msg,const char *buf);

/*
struct Buffer *new_buffer();
void append_space(Buffer *b,int n);
void serialize_int(int x,Buffer *b);
void serialize_string(char *str,Buffer *b);
void serialize_message(struct message *msg,Buffer *b);
*/

#endif

