#include "server.h"
#include "sock.h"

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
  return handle_incoming_request(sockfd);
}


void get_host_addr(const char *port, struct addrinfo **res)
{
  struct addrinfo hints;
  int64_t rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, port, &hints, res)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(1);
  }
}


int64_t handle_incoming_request(int64_t sockfd)
{
  struct sockaddr_storage their_addr;
  char s[INET6_ADDRSTRLEN];
  int64_t new_fd;

  while (1)
  {
    if ((new_fd = sock_accept(sockfd, &their_addr, s)) == -1)
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
  int64_t len = 250;
  char buf[249];
  int64_t bytes_read;

  while (1)
  {
    bytes_read = recv(fd, buf, len, 0);
    if (bytes_read < 1)
    {
      break;
    }
    print_buf(buf, bytes_read);
    if (bytes_read < len)
    {
      break;
    }
  }
  return;
}

void print_buf(char * buf, int64_t len)
{
  int64_t i;
  for (i = 0; i < len; i++)
  {
    printf("%c", buf[i]);
  }
  printf("\n");

}

/* make sure we do not get any zombie processes*/
void sigchld_handler(int s)
{
  int saved_errno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0);
  errno = saved_errno;
}

