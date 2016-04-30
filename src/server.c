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
    exit_with_user_message("server:", "bind failed");
  }

  sock_listen(sockfd, BACKLOG);

  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  if (sigaction(SIGCHLD, &sa, NULL) == -1)
  {
    exit_with_system_message("server: sigaction");
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
    exit_with_user_message("server: getaddrinfo", gai_strerror(rv));
  }
}

int64_t handle_incoming_request(int64_t sockfd)
{
  struct sockaddr_storage client_addr;
      session_t client;
  int64_t new_fd;
  const char msg[] = "Request processed.\n";

  while (1)
  {
    if ((new_fd = sock_accept(sockfd, &client_addr)) == -1)
    {
      continue;
    }
    if (!fork())
    {
      sock_close(sockfd);
      client.client_socket = new_fd;
      read_request(&client);
      if (send(new_fd, msg, sizeof msg, 0) == -1)
      {
        system_message("server: send to client", 0);
      }
      free_session(&client);
      exit(0);
    }
    sock_close(new_fd);
  }
  return 0;
}

void read_request(session_t *session)
{
  int64_t BUFSIZE = 250;
  char buf[BUFSIZE];
  int64_t bytes_read;

  /* TODO: error check recv */
  bytes_read = recv(session->client_socket, buf, BUFSIZE - 1, 0);
  while (bytes_read > 0)
  {
    buf[bytes_read] = '\0';
    fputs(buf, stdout);
    if (bytes_read < BUFSIZE - 1)
    {
      break;
    }
    bytes_read = recv(session->client_socket, buf, BUFSIZE - 1, 0);
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

