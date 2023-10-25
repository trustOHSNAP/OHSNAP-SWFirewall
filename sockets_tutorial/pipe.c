#include <stdio.h>
// These fix warnings from the original source
// Figure 1: https://stevens.netmeister.org/631/ipctut.pdf
#include <unistd.h>
#include <stdlib.h>

#define DATA "Bright star, would I were steadfast as thou art . . ."

/*
 * This program creates a pipe, then forks.  The child communicates to the
 * parent over the pipe. Notice that a pipe is a one-way communications
 * device.  I can write to the output socket (sockets[1], the second socket
 * of the array returned by pipe()) and read from the input socket
 * (sockets[0]), but not vice versa.
 */
int main() {
	int sockets[2], child;

	/* Create a pipe */
	if (pipe(sockets) < 0) {
		perror("opening stream socket pair");
		exit(10);
	}

	if ((child = fork()) == -1) {
		perror("fork");
	} else if (child) {
		char buf[1024];

		// This is still the parent. It reads the child's message.
		close(sockets[1]);
		if (read(sockets[0], buf, 1024) < 0) {
			perror("reading message");
		}
		printf("-->%s\n", buf);
		close(sockets[0]);
	} else {
		// This is the child. It writes its message to the parent.
		close(sockets[0]);
		if (write(sockets[1], DATA, sizeof(DATA)) < 0) {
			perror("writing message");
		}
		close(sockets[1]);

	}
}
