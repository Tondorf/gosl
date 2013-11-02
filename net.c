
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

#define PORT "4711"

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main (int argc, char **argv) {

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;      // IPv4
	hints.ai_socktype = SOCK_DGRAM; // UDP
	hints.ai_flags = AI_PASSIVE;

	int rv;
	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	printf("got local address\n");
	
	struct addrinfo *info;
	int sockfd;
	char s[INET6_ADDRSTRLEN];
	for (info = servinfo; info != NULL; info = info->ai_next) {
/*		if (info->ai_addr) {
			struct sockaddr_in* p = (struct sockaddr_in *)info->ai_addr;
			inet_ntop(p->sin_family, &p->sin_addr, s, INET6_ADDRSTRLEN); 
			printf("  %d: %s\n", info->ai_addr->sa_family, s);
		}
*/

		if ((sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) == -1) {
			perror("sock");
			continue; 
		}

		if (bind(sockfd, info->ai_addr, info->ai_addrlen) == -1) {
			perror("bind");
			continue; 
		}

		break;
	}
	if (!info) {
		fprintf(stderr, "unbound\n");
		return 2;
	}
	printf("check!\n");

	freeaddrinfo(servinfo); // free whole list
 
	struct sockaddr_storage their_addr;
	socklen_t addr_len = sizeof their_addr;
	int numbytes;
	char buf[10000];
	do {
		if ((numbytes = recvfrom(sockfd, buf, 9999 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
	    	perror("recvfrom");
		    exit(1);
		}
		
		printf("listener: got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
		printf("listener: packet is %d bytes long\n", numbytes);
		buf[numbytes] = '\0';
		printf("listener: packet contains \"%s\"\n", buf);
	} while (strncmp(buf, "exit", 10000));


	close(sockfd);
	
	return 0;
}

