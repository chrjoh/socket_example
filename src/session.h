#pragma once

#include <stdlib.h>
#include "sock.h"

typedef struct session
{
  char *request, *request_uri;
  size_t bytes_in_buffer, header_length, buffer_size, uri_length;
  int64_t client_socket;
} session_t;

void free_session(session_t *session);
void setup_session(session_t *session, int64_t fd);
void check_and_free(void *p);
void print_session(session_t *session);

