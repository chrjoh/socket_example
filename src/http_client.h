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
#include "sock.h"
#include "logging.h"
#include "session.h"

void handle_client(int64_t fd);
void read_request(session_t *session);
int64_t extract_request_into_session(session_t *session);
void set_header_length(session_t *session);
