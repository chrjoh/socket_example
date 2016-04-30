#pragma once

typedef struct type_session {
  char []raw_request, []request_uri;
  size_t bytes_in_buffer, header_length, buffer_size, uri_length;
  int64_t client_socket;
} t_session;
  
