#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
void read_request(int64_t fd);
int64_t handle_incoming_request(int64_t socket_fd);
void get_host_addr(char port[], struct addrinfo **res);


#endif
