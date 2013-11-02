
#include "display.h"

void callback(const struct message *msg) {
	printf("in callback, tst=%d\n", msg->timestamp);
}
