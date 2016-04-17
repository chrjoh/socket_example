#include "sock.h"

void sock_listen(int64_t sockfd, int64_t backlog)
{
  if (listen(sockfd, backlog) == -1)
  {
    exit_with_system_message("sock: listen");
  }
}

void sock_bind(int64_t sockfd, struct addrinfo *p)
{
  if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
  {
    sock_close(sockfd);
    exit_with_system_message("sock: bind");
  }
}

int64_t sock_create(struct addrinfo *p)
{
  int64_t sockfd;
  int64_t yes = 1;

  if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
  {
    exit_with_system_message("sock: create");
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int64_t)) == -1)
  {
    exit_with_system_message("sock: set socket options");
  }
  return sockfd;
}

int64_t sock_accept(int64_t sockfd, struct sockaddr_storage *client_addr)
{
  socklen_t sin_size;
  int64_t new_fd;
  char s[INET6_ADDRSTRLEN];

  sin_size = sizeof client_addr;
  new_fd = accept(sockfd, (struct sockaddr *)client_addr, &sin_size);

  if (new_fd == -1)
  {
    return system_message("sock: error then accepting client socket", -1);
  }

  inet_ntop(client_addr->ss_family, sock_get_in_addr((struct sockaddr *)client_addr), s, sizeof s);

  printf("server: got connection from %s\n", s);
  return new_fd;
}

int64_t sock_read(int64_t socketd, char buf[], int64_t n)
{
  int64_t nread;

  nread = read(socketd, buf, n);

  if (nread < 0)
  {
    exit_with_system_message("sock: read");
  }

  return nread;
}

int64_t sock_write(int64_t socketd, char buf[], int64_t n)
{
  int64_t nwritten;

  nwritten = write(socketd, buf, n);

  if (nwritten != n)
  {
    exit_with_system_message("sock: write");
  }

  return nwritten;
}

int64_t sock_close(int64_t socketd)
{
  int64_t   res;

  res = close(socketd);

  if (res < 0)
  {
    exit_with_system_message("sock: close");
  }

  return res;
}


/* sock_readchrd
 *
 * Sock_readchrd reads characters into the given
 * buffer until either n - 1 characters are read,
 * the given character delimiter is read and put
 * in the buffer, read returns -1,
 * or read returns 0.
 *
 * The sequence of characters that has been read
 * is nul-terminated.
 *
 * Sock_readchrd returns the number of
 * characters read.
 *
 */
int64_t sock_readchrd(int64_t socketd, char buf[], int64_t n, char delim)
{
  int64_t   nread;
  char *start = buf;

  while (buf - start < n - 1
         &&  (nread = read(socketd, buf, 1)) == 1
         &&  *buf != delim)
  {
    ++buf;
  }

  if (nread < 0)
  {
    exit_with_system_message("sock: error then reading with char delimiter");
  }

  if (*buf == delim)
    ++buf;

  *buf = '\0';

  return  buf - start;
}

/* sock_readstrd
 *
 * Sock_readstrd reads characters into the given
 * buffer until either n - 1 characters are read,
 * the given string delimiter is read and put in
 * the buffer, read returns -1, or read returns 0.
 *
 * The sequence of characters that has been read
 * is nul-terminated.
 *
 * Sock_readstrd returns the number of
 * characters read.
 *
 */

int64_t sock_readstrd(int64_t socketd, char buf[], int64_t n, char delim[])
{
  int64_t   nread;
  int64_t   i = 0;
  int64_t   dlen = strlen(delim);

  if (n == 0)
  {
    return 0;
  }
  else if (n == 1)
  {
    buf[0] = '\0';
    return 0;
  }

  nread = read(socketd, buf, 1);
  buf[++i] = '\0';

  while (nread == 1
         &&  i != n - 1
         &&  (i < dlen
              ||  strncmp(buf + i - dlen, delim, dlen) != 0))
  {
    nread = read(socketd, buf + i, 1);

    if (nread == 1)
      ++i;

    buf[i] = '\0';
  }

  if (nread < 0)
  {
    exit_with_system_message("sock: error then reading with string delimiter");
  }
  return  i;
}

/* helper functions */
void *sock_get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
