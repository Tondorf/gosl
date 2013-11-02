#ifndef __NET
#  define __NET

#include <sys/socket.h> // struct sockaddr
#include "misc.h"     // prog_info
#include "msg.h"      // message serialization
#include "display.h"  // callback 

void *get_in_addr(struct sockaddr *sa);
int run_server(const struct prog_info *pinfo);
int run_client(const struct prog_info *pinfo,void(*framecallback)(const struct message *, const struct prog_info *));

#endif
