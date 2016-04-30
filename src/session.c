#include "session.h"

void free_session(session_t *session)
{
  check_and_free(session->request_uri);
  check_and_free(session->request);
}

void setup_session(session_t *session)
{
  session->request = NULL;
  session->request_uri = NULL;
  session->client_socket = 0;
  session->header_length = 0;
  session->buffer_size = 0;
  session->bytes_in_buffer = 0;
  session->uri_length = 0;
}

void check_and_free(void *p)
{
  if (p != NULL)
  {
    free(p);
  }
}

