#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "sock.h"

void sock_listen(int sockfd, int backlog)
{
  if (listen(sockfd, backlog) == -1) {
    perror("listen");
    exit(1);
  }
}

void sock_bind(int sockfd, struct addrinfo *p)
{
  if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
    close(sockfd);
    perror("server: bind");
    exit(1);
  }
}

int sock_create(struct addrinfo *p)
{
  int sockfd;
  int yes = 1;

  if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
    perror("server: socket");
    exit(1);
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
  }
  return sockfd;
}

int sock_accept(int sockfd, struct sockaddr_storage *their_addr, char *s)
{
  socklen_t sin_size;
  int new_fd;
  sin_size = sizeof their_addr;
  new_fd = accept(sockfd, (struct sockaddr *)their_addr, &sin_size);
  if (new_fd == -1) {
    perror("accept");
    return -1;
  }
  inet_ntop(their_addr->ss_family, sock_get_in_addr((struct sockaddr *)their_addr), s, sizeof s);
  printf("server: got connection from %s\n", s);
  return new_fd;
}

int sock_read(int socketd, char *buf, int n)
{
  int nread;

  nread = read(socketd, buf, n);

  if (nread < 0)
  {
    perror("read");
    exit(1);
  }

  return nread;
}

int sock_write(int socketd, char *buf, int n)
{
  int nwritten;

  nwritten = write(socketd, buf, n);

  if (nwritten != n)
  {
    perror("write");
    exit (1);
  }

  return nwritten;
}

int sock_close(int socketd)
{
  int   res;

  res = close(socketd);

  if (res < 0)
  {
    perror("close");
    exit(1);
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
int sock_readchrd(int socketd, char *buf, int n, char delim)
{
  int   nread;
  char *start = buf;

  while (buf - start < n - 1
      &&  (nread = read(socketd, buf, 1)) == 1
      &&  *buf != delim)
  {
    ++buf;
  }

  if (nread < 0)
  {
    perror("read");
    exit (1);
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

int sock_readstrd(int socketd, char *buf, int n, char *delim)
{
  int   nread;
  int   i = 0;
  int   dlen = strlen(delim);

  if (n == 0)
    return 0;
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
        ||  strncmp(buf +i -dlen, delim, dlen) != 0))
  {
    nread = read(socketd, buf + i, 1);

    if (nread == 1)
      ++i;

    buf[i] = '\0';
  }

  if (nread < 0)
  {
    perror("read");
    exit(1);
  }
  return  i;
}

/* helper functions */
void *sock_get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
