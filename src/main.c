#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


#include "misc.h"
#include "net.h"
#include "display.h"

#define MODE_SERVER 0
#define MODE_CLIENT 1


int parseArgs(struct prog_info *pinfo, int argc, char **argv) {
	pinfo->mode = MODE_SERVER;
	pinfo->client_offset = 1;
	pinfo->port = 4711;
	pinfo->fps = 10;

	if (argc <= 1) {
		printf("usage: %s [-s|-c <off>] [-p <port>] [-t <fps>]\n", argv[0]);
		printf("where:\n -s         run in server mode\n");
		printf(        " -c <off>   run in client mode\n");
		printf(        "            off is the column offset to use.\n");
		printf(        " -p <port>  use the specified port\n");
		printf(        " -t <fps>   when in server mode: update <fps> times\n"); 
		printf(        "            per second. Valid range: 2 - 99\n");
		printf(        "            ignored in client mode\n");
		printf(        " -v          \n");
		printf("\n\n");
		return -1;
	}
	int i;
	for (i=1; i<argc; i++) {
		if (strncmp(argv[i], "-s", 2) == 0) {
			pinfo->mode = MODE_SERVER;
			continue;
		}
		if (strncmp(argv[i], "-c", 2) == 0) {
			pinfo->mode = MODE_CLIENT;
			if (argc <= i+1) {
				printf("client number not specified\n");
				return -2;
			}
			pinfo->client_offset = (int)strtol(argv[++i], NULL, 10);
			if (pinfo->client_offset <= 0) {
				printf("invalid client number!\n");
				return -3;
			}
			continue;
		}
		if (strncmp(argv[i], "-p", 2) == 0) {
			if (argc <= i+1) {
				printf("port not specified\n");
				return -4;
			}
			pinfo->port = (int)strtol(argv[++i], NULL, 10);
			if (pinfo->port <= 0 || 65536 <= pinfo->port) {
				printf("invalid port number!\n");
				return -5;
			}
			continue;
		}
		if (strncmp(argv[i], "-t", 2) == 0) {
			if (argc <= i+1) {
				printf("fps not specified\n");
				return -6;
			}
			pinfo->fps = (int)strtol(argv[++i], NULL, 10);
			if (pinfo->fps <= 1 || 100 <= pinfo->fps) {
				printf("fps invalid!\n");
				return -7;
			}
			continue;
			
		}
		printf("unknown argument %s\n", argv[i]);
		return -6;
	}

	return 0;
}


int main(int argc, char **argv) {

	int ret;
	if ((ret = parseArgs(&prog_info, argc, argv)) == 0) {
			
		printf("port: %d\n", prog_info.port);
		if (prog_info.mode == MODE_SERVER) {
			printf("running in SERVER mode @%d FPS\n", prog_info.fps);
			ret = run_server(&prog_info);
		} else {
			printf("running in CLIENT mode, using client number %d\n", prog_info.client_offset);
			signal(SIGINT,&cleanup_display);
			ret = run_client(&prog_info, callback);
		}

	}
	return ret;
}

