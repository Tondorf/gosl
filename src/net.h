/* This file was automatically generated.  Do not edit! */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
int run_client(const struct prog_info *pinfo,void(*framecallback)(long));
int run_server(const struct prog_info *pinfo);
void *get_in_addr(struct sockaddr *sa);
