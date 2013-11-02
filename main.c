#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "misc.h"
#include "net.c"


#define MODE_SERVER 0
#define MODE_CLIENT 1


int parseArgs(struct prog_info *pinfo, int argc, char **argv) {
	pinfo->mode = MODE_SERVER;
	pinfo->client_num = 1;
	pinfo->port = 4711;

	if (argc <= 1) {
		printf("usage: %s [-s|-c <num>] [-p <port>]\n", argv[0]);
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
			pinfo->client_num = (int)strtol(argv[++i], NULL, 10);
			if (pinfo->client_num <= 0) {
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
		printf("unknown argument %s\n", argv[i]);
		return -6;
	}

	return 0;
}

void callback(long tst) {
	printf("in callback, tst=%ld\n", tst);
}

int main(int argc, char **argv) {

	int ret;
	if ((ret = parseArgs(&prog_info, argc, argv)) == 0) {
			
		printf("port: %d\n", prog_info.port);
		if (prog_info.mode == MODE_SERVER) {
			printf("runnin in SERVER mode\n");
			ret = run_server(&prog_info);
		} else {
			printf("running in CLIENT mode, using client number %d\n", prog_info.client_num);
			// ...
			ret = run_client(&prog_info, callback);
		}

	}
	return ret;
}

