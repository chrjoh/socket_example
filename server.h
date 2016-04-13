#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "sock.h"

#define PORT "7071"

#define BACKLOG 10

void sigchld_handler(int s);
void read_request(int fd);
void print_buf(char * buf, int len);
int handle_incoming_request(int socket_fd);
void get_host_addr(char *port, struct addrinfo **res);


#endif
