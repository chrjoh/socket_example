#include "session.h"


void free_session(session_t *session)
{
  check_and_free(session->request_uri);
  check_and_free(session->request);
  if (session->client_socket > 0)
  {
    sock_close(session->client_socket);
  }
}

void setup_session(session_t *session, int64_t fd)
{
  session->request = NULL;
  session->request_uri = NULL;
  session->client_socket = fd;
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

void print_session(session_t *session)
{
  printf("--------------------S T A R T-------------------------------\n");
  printf("---> REQUEST START\n");
  fputs(session->request, stdout);
  fputc('\n', stdout);
  printf("---> REQUEST END\n");
  printf("Bytes in buffer: %" PRId64 "\n", session->bytes_in_buffer);
  printf("Header length: %" PRId64 "\n", session->header_length);
  printf("---> HEADER START\n");
  for (int i = 0; i < session->header_length; i++)
  {
    fputc(session->request[i], stdout);
  }
  printf("---> HEADER END\n");
  printf("---> BODY START\n");
  for (int i = session->header_length; i < session->bytes_in_buffer; i++)
  {
    fputc(session->request[i], stdout);
  }
  printf("\n---> BODY END\n");
  printf("---------------------E N D----------------------------------\n");
}
