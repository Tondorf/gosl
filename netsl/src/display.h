#ifndef __DISPLAY
#define __DISPLAY

#include <stdio.h>
#include "msg.h"
#include "misc.h"

void setup_display();
void cleanup_display();

void callback(const struct message *msg, const struct prog_info *pinfo);

#endif

