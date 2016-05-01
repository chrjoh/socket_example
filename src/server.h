#pragma once

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
#include <pthread.h>
#include "sock.h"
#include "logging.h"
#include "session.h"
#include "http_client.h"

#define PORT "7071"
#define BACKLOG 10

typedef struct thread_args
{
  int64_t client_socket;
} thread_args_t;

void sigchld_handler(int s);
int64_t handle_incoming_request(int64_t socket_fd);
void get_host_addr(char port[], struct addrinfo **res);
void *thread_worker(void *thread_args);
