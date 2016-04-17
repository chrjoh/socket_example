#include "server.h"

int main(void)
{
  int64_t sockfd;
  struct addrinfo *servinfo, *p;
  struct sigaction sa;

  get_host_addr(PORT, &servinfo);

  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    sockfd = sock_create(p);
    sock_bind(sockfd, p);
    break;
  }

  freeaddrinfo(servinfo);

  if (p == NULL)
  {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  sock_listen(sockfd, BACKLOG);

  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  if (sigaction(SIGCHLD, &sa, NULL) == -1)
  {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connections...\n");
  handle_incoming_request(sockfd);
  sock_close(sockfd);
  return 0;
}

void get_host_addr(char port[], struct addrinfo **res)
{
  struct addrinfo hints = {0};
  int64_t rv;

  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags    = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, port, &hints, res)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(1);
  }
}

int64_t handle_incoming_request(int64_t sockfd)
{
  struct sockaddr_storage client_addr;
  int64_t new_fd;

  while (1)
  {
    if ((new_fd = sock_accept(sockfd, &client_addr)) == -1)
    {
      continue;
    }
    if (!fork())
    {
      sock_close(sockfd);
      read_request(new_fd);
      if (send(new_fd, "Done", 13, 0) == -1)
      {
        perror("send");
      }
      sock_close(new_fd);
      exit(0);
    }
    sock_close(new_fd);
  }
  return 0;
}

void read_request(int64_t fd)
{
  int64_t BUFSIZE = 250;
  char buf[BUFSIZE];
  int64_t bytes_read;

  bytes_read = recv(fd, buf, BUFSIZE - 1, 0);
  while (bytes_read > 0)
  {
    buf[bytes_read] = '\0';
    fputs(buf, stdout);
    if (bytes_read < BUFSIZE - 1)
    {
      break;
    }
    bytes_read = recv(fd, buf, BUFSIZE - 1, 0);
  }
  fputc('\n', stdout);
  return;
}


/* make sure we do not get any zombie processes*/
void sigchld_handler(int s)
{
  int saved_errno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0);
  errno = saved_errno;
}

