#ifndef __NET
#  define __NET

#include <sys/socket.h> // struct sockaddr

void *get_in_addr(struct sockaddr *sa);
int run_server(const struct prog_info *pinfo);
int run_client(const struct prog_info *pinfo,void(*framecallback)(long));

#endif
