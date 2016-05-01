#include "http_client.h"

#define BUFFER_CHUNK     64

void handle_client(int64_t fd)
{
  const char msg[] = "Request processed.\n";
  session_t client;

  setup_session(&client, fd);
  read_request(&client);
  set_header_length(&client);
  extract_request_into_session(&client);
  print_session(&client);
  if (send(fd, msg, sizeof msg, 0) == -1)
  {
    system_message("server: send to client", 0);
  }
  free_session(&client);
}

void set_header_length(session_t *session)
{
  char *body_start;
  if ((body_start = strstr(session->request, "\r\n\r\n")) != NULL)
  {
    session->header_length = body_start + 4 - session->request;
  }
}

void read_request(session_t *session)
{
  char *buf;
  int64_t bytes_read;

  session->buffer_size = BUFFER_CHUNK;
  if ((buf = (char*)realloc(session->request, session->buffer_size + 1)) != NULL)
  {
    session->request = buf;
  }
  else
  {
    exit_with_system_message("server: failed to allocate buffer memory");
  }

  /* TODO: error check recv
   * clean up code and move first allocate and read into the while loop
   * verify pointer calculations!!
   * */
  bytes_read = recv(session->client_socket, session->request, session->buffer_size, 0);
  while (bytes_read > 0)
  {
    session->bytes_in_buffer += bytes_read;
    //printf("Bytes in buffer: %" PRId64 "\n",session->bytes_in_buffer);
    session->buffer_size += BUFFER_CHUNK;
    if (bytes_read < BUFFER_CHUNK - 1)
    {
      session->request[session->bytes_in_buffer] = '\0';
      break;
    }
    if ((buf = (char*)realloc(session->request, session->buffer_size + 1)) != NULL)
    {
      session->request = buf;
    }
    else
    {
      exit_with_system_message("server: failed to allocate buffer memory");
    }
    bytes_read = recv(session->client_socket,
                      session->request + session->bytes_in_buffer,
                      session->buffer_size - session->bytes_in_buffer, 0);
  }
  return;
}


int64_t extract_request_into_session(session_t *session)
{
  char *str, *header_end;

  header_end = session->request + session->header_length;

  session->method = str = session->request;
  while ((*str != ' ') && (str != header_end))
  {
    str++;
  }
  *str = '\0';// insert string termination
  session->uri = ++str;

  while ((*str != ' ') && (str != header_end))
  {
    str++;
  }
  *str = '\0';
  session->uri_length = strlen(session->uri);

  if ((session->request_uri = strdup(session->uri)) == NULL)
  {
    return -1;
  }
  str++; // we have parsed out the uri so point the str to next value in the request

  session->body = session->request + session->header_length;
  /* TODO: add other requst header values as needed also need error checks*/
  return 0;
}
