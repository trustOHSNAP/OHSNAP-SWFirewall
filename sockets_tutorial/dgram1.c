#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*
 * In the included file <sys/un.h> a sockaddr_un is defined as follows
 * struct sockaddr_un {
 *    short sun_family;
 *    char  sun_path[108];
 * };
 */
#include <stdio.h>
#define NAME "socket"
/*
 * This program creates a UNIX domain datagram socket, binds a name to it,
 * then reads from the socket.
 */
int main() {
	int sock, length;
	struct sockaddr_un name;
	char buf[1024];
	/* Create socket from which to read. */
	sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock < 0) {
      		perror("opening datagram socket");
      		exit(1);
	}
	/* Create name. */
	name.sun_family = AF_UNIX;
	strcpy(name.sun_path, NAME);
	//if (bind(sock, &name, sizeof(struct sockaddr_un))) {
	// Fix warning, but not sure it's a good idea.
	if (bind(sock, (const struct sockaddr*)&name, sizeof(struct sockaddr_un))) {
      		perror("binding name to datagram socket");
      		exit(1);
	}
	printf("socket -->%s\n", NAME);
	/* Read from the socket */
	if (read(sock, buf, 1024) < 0) {
      		perror("receiving datagram packet");
	}
	printf("-->%s\n", buf);
	// important!
	close(sock);
	unlink(NAME);
}
