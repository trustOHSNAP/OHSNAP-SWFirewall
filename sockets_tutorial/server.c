/*
 * Server code
 * Compile it with: cc -Wall -Wextra -Werror server.c -o server
 * https://marcocetica.com/posts/socket_tutorial_part1/
 */
#include <stdio.h> // printf, puts
#include <stdlib.h> // memset, strlen
#include <string.h> // strlen
#include <unistd.h> // write
#include <sys/socket.h>
#include <arpa/inet.h>
// This is for BSD for socketaddr_in
// I learned this from https://stevens.netmeister.org/631/ipctut.pdf
#include <netinet/in.h>

#define BUF_SIZE 1024

int main(int argc, char **argv) {
    int server_fd = 0, client_fd = 0, read_sz, count = 0;
    struct sockaddr_in server_sock, client_sock;
    char buf[BUF_SIZE];

    // Check for cli arguments
    if(argc != 2) {
        printf("Usage: %s <SERVER_PORT>\n", argv[0]);
        return 1;
    }

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1) {
        puts("Unable to create socket");
        return 1;
    }

    // Setup socket and initialize buffer
    server_sock.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_sock.sin_port = htons(atoi(argv[1]));
    server_sock.sin_family = AF_INET;
    memset(buf, 0, sizeof(buf));

    // 2. Bind socket to address and port
    int ret = bind(server_fd, (struct sockaddr*)&server_sock, sizeof(server_sock));
    if(ret < 0) {
        puts("Unable to bind TCP socket");
        return 1;
    }

    // 3. Listen for connections
    // 2nd parameter is the backlog. i.e., the maximum length to which the queue
    // of pending connections may grow
    listen(server_fd, 128);
    int len = sizeof(struct sockaddr_in);

    for(;;) {
        // 4. Accept incoming connections
        client_fd = accept(server_fd, 
                    (struct sockaddr*)&client_sock, 
                    (socklen_t*)&len);

        if(client_fd < 0) {
            puts("Unable to accept connections");
            return 1;
        }
        // Print client IP address
        char *client_addr = inet_ntoa(client_sock.sin_addr);
        int client_port = ntohs(client_sock.sin_port);
        printf("New connection from %s:%d\n", client_addr, client_port);
        // 5. Send data back to clients
        snprintf(buf, 
                sizeof(buf), 
                "server> You've reached me %d time(s)\n", 
                ++count);
        write(client_fd, buf, strlen(buf));
    }

    if(read_sz == -1) {
        puts("Error reading from client");
        return 1;
    }

    return 0;
}

