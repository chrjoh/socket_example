#include "server.h"

int main(void)
{
  int64_t sockfd = 0;
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
  int64_t new_fd;

  while (1)
  {
    if ((new_fd = sock_accept(sockfd, &client_addr)) == -1)
    {
      continue;
    }
    thread_args_t *thread_args = (thread_args_t *) malloc(sizeof(thread_args_t));
    if (thread_args == NULL)
    {
      exit_with_system_message("server: failed to allocate memory for thread");
    }
    thread_args->client_socket = new_fd;
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, thread_worker, thread_args) != 0)
    {
      exit_with_system_message("server: failed to create thread");
    }

  }
  sock_close(sockfd);
  return 0;
}

void *thread_worker(void *thread_args)
{
  pthread_detach(pthread_self());
  int64_t fd = ((thread_args_t *) thread_args)->client_socket;
  free(thread_args);
  handle_client(fd);

  return NULL;
}

/* make sure we do not get any zombie processes*/
void sigchld_handler(int s)
{
  int saved_errno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0);
  errno = saved_errno;
}

