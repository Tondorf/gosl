#ifndef __DISPLAY
#define __DISPLAY

#include <stdio.h>
#include "msg.h"

void setup_display();
void cleanup_display();

void callback(const struct message *msg);

#endif

